/*
 * linex/arch/arm/mach-omap1/timer32k.c
 *
 * OMAP 32K Timer
 *
 * Copyright (C) 2004 - 2005 Nokia Corporation
 * Partial timer rewrite and additional dynamic tick timer support by
 * Tony Lindgen <tony@atomide.com> and
 * Tuukka Tikkanen <tuukka.tikkanen@elektrobit.com>
 * OMAP Dual-mode timer framework support by Timo Teras
 *
 * MPU timer code based on the older MPU timer code for OMAP
 * Copyright (C) 2000 RidgeRun, Inc.
 * Author: Greg Lonnon <glonnon@ridgerun.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/delay.h>
#include <linex/interrupt.h>
#include <linex/sched.h>
#include <linex/spinlock.h>
#include <linex/err.h>
#include <linex/clk.h>
#include <linex/clocksource.h>
#include <linex/clockchips.h>
#include <linex/io.h>
#include <linex/sched_clock.h>

#include <asm/irq.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include "hardware.h"
#include "common.h"

/*
 * ---------------------------------------------------------------------------
 * 32KHz OS timer
 *
 * This currently works only on 16xx, as 1510 does not have the continuous
 * 32KHz synchronous timer. The 32KHz synchronous timer is used to keep track
 * of time in addition to the 32KHz OS timer. Using only the 32KHz OS timer
 * on 1510 would be possible, but the timer would not be as accurate as
 * with the 32KHz synchronized timer.
 * ---------------------------------------------------------------------------
 */

/* 16xx specific defines */
#define OMAP1_32K_TIMER_BASE		0xfffb9000
#define OMAP1_32KSYNC_TIMER_BASE	0xfffbc400
#define OMAP1_32K_TIMER_CR		0x08
#define OMAP1_32K_TIMER_TVR		0x00
#define OMAP1_32K_TIMER_TCR		0x04

#define OMAP_32K_TICKS_PER_SEC		(32768)

/*
 * TRM says 1 / HZ = ( TVR + 1) / 32768, so TRV = (32768 / HZ) - 1
 * so with HZ = 128, TVR = 255.
 */
#define OMAP_32K_TIMER_TICK_PERIOD	((OMAP_32K_TICKS_PER_SEC / HZ) - 1)

#define JIFFIES_TO_HW_TICKS(nr_jiffies, clock_rate)			\
				(((nr_jiffies) * (clock_rate)) / HZ)

static inline void omap_32k_timer_write(int val, int reg)
{
	omap_writew(val, OMAP1_32K_TIMER_BASE + reg);
}

static inline void omap_32k_timer_start(unsigned long load_val)
{
	if (!load_val)
		load_val = 1;
	omap_32k_timer_write(load_val, OMAP1_32K_TIMER_TVR);
	omap_32k_timer_write(0x0f, OMAP1_32K_TIMER_CR);
}

static inline void omap_32k_timer_stop(void)
{
	omap_32k_timer_write(0x0, OMAP1_32K_TIMER_CR);
}

#define omap_32k_timer_ack_irq()

static int omap_32k_timer_set_next_event(unsigned long delta,
					 struct clock_event_device *dev)
{
	omap_32k_timer_start(delta);

	return 0;
}

static int omap_32k_timer_shutdown(struct clock_event_device *evt)
{
	omap_32k_timer_stop();
	return 0;
}

static int omap_32k_timer_set_periodic(struct clock_event_device *evt)
{
	omap_32k_timer_stop();
	omap_32k_timer_start(OMAP_32K_TIMER_TICK_PERIOD);
	return 0;
}

static struct clock_event_device clockevent_32k_timer = {
	.name			= "32k-timer",
	.features		= CLOCK_EVT_FEAT_PERIODIC |
				  CLOCK_EVT_FEAT_ONESHOT,
	.set_next_event		= omap_32k_timer_set_next_event,
	.set_state_shutdown	= omap_32k_timer_shutdown,
	.set_state_periodic	= omap_32k_timer_set_periodic,
	.set_state_oneshot	= omap_32k_timer_shutdown,
	.tick_resume		= omap_32k_timer_shutdown,
};

static irqreturn_t omap_32k_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &clockevent_32k_timer;
	omap_32k_timer_ack_irq();

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static __init void omap_init_32k_timer(void)
{
	if (request_irq(INT_OS_TIMER, omap_32k_timer_interrupt,
			IRQF_TIMER | IRQF_IRQPOLL, "32KHz timer", NULL))
		pr_err("Failed to request irq %d(32KHz timer)\n", INT_OS_TIMER);

	clockevent_32k_timer.cpumask = cpumask_of(0);
	clockevents_config_and_register(&clockevent_32k_timer,
					OMAP_32K_TICKS_PER_SEC, 1, 0xfffffffe);
}

/* OMAP2_32KSYNCNT_CR_OFF: offset of 32ksync counter register */
#define OMAP2_32KSYNCNT_REV_OFF		0x0
#define OMAP2_32KSYNCNT_REV_SCHEME	(0x3 << 30)
#define OMAP2_32KSYNCNT_CR_OFF_LOW	0x10
#define OMAP2_32KSYNCNT_CR_OFF_HIGH	0x30

/*
 * 32KHz clocksource ... always available, on pretty most chips except
 * OMAP 730 and 1510.  Other timers could be used as clocksources, with
 * higher resolution in free-running counter modes (e.g. 12 MHz xtal),
 * but systems won't necessarily want to spend resources that way.
 */
static void __iomem *sync32k_cnt_reg;

static u64 notrace omap_32k_read_sched_clock(void)
{
	return sync32k_cnt_reg ? readl_relaxed(sync32k_cnt_reg) : 0;
}

/**
 * omap_read_persistent_clock64 -  Return time from a persistent clock.
 *
 * Reads the time from a source which isn't disabled during PM, the
 * 32k sync timer.  Convert the cycles elapsed since last read into
 * nsecs and adds to a monotonically increasing timespec64.
 */
static struct timespec64 persistent_ts;
static cycles_t cycles;
static unsigned int persistent_mult, persistent_shift;

static void omap_read_persistent_clock64(struct timespec64 *ts)
{
	unsigned long long nsecs;
	cycles_t last_cycles;

	last_cycles = cycles;
	cycles = sync32k_cnt_reg ? readl_relaxed(sync32k_cnt_reg) : 0;

	nsecs = clocksource_cyc2ns(cycles - last_cycles,
					persistent_mult, persistent_shift);

	timespec64_add_ns(&persistent_ts, nsecs);

	*ts = persistent_ts;
}

/**
 * omap_init_clocksource_32k - setup and register counter 32k as a
 * kernel clocksource
 * @pbase: base addr of counter_32k module
 * @size: size of counter_32k to map
 *
 * Returns 0 upon success or negative error code upon failure.
 *
 */
static int __init omap_init_clocksource_32k(void __iomem *vbase)
{
	int ret;

	/*
	 * 32k sync Counter IP register offsets vary between the
	 * highlander version and the legacy ones.
	 * The 'SCHEME' bits(30-31) of the revision register is used
	 * to identify the version.
	 */
	if (readl_relaxed(vbase + OMAP2_32KSYNCNT_REV_OFF) &
						OMAP2_32KSYNCNT_REV_SCHEME)
		sync32k_cnt_reg = vbase + OMAP2_32KSYNCNT_CR_OFF_HIGH;
	else
		sync32k_cnt_reg = vbase + OMAP2_32KSYNCNT_CR_OFF_LOW;

	/*
	 * 120000 rough estimate from the calculations in
	 * __clocksource_update_freq_scale.
	 */
	clocks_calc_mult_shift(&persistent_mult, &persistent_shift,
			32768, NSEC_PER_SEC, 120000);

	ret = clocksource_mmio_init(sync32k_cnt_reg, "32k_counter", 32768,
				250, 32, clocksource_mmio_readl_up);
	if (ret) {
		pr_err("32k_counter: can't register clocksource\n");
		return ret;
	}

	sched_clock_register(omap_32k_read_sched_clock, 32, 32768);
	register_persistent_clock(omap_read_persistent_clock64);
	pr_info("OMAP clocksource: 32k_counter at 32768 Hz\n");

	return 0;
}

/*
 * ---------------------------------------------------------------------------
 * Timer initialization
 * ---------------------------------------------------------------------------
 */
int __init omap_32k_timer_init(void)
{
	int ret = -ENODEV;

	if (cpu_is_omap16xx()) {
		void __iomem *base;
		struct clk *sync32k_ick;

		base = ioremap(OMAP1_32KSYNC_TIMER_BASE, SZ_1K);
		if (!base) {
			pr_err("32k_counter: failed to map base addr\n");
			return -ENODEV;
		}

		sync32k_ick = clk_get(NULL, "omap_32ksync_ick");
		if (!IS_ERR(sync32k_ick))
			clk_prepare_enable(sync32k_ick);

		ret = omap_init_clocksource_32k(base);
	}

	if (!ret)
		omap_init_32k_timer();

	return ret;
}
