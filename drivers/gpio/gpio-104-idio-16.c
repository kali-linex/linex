// SPDX-License-Identifier: GPL-2.0-only
/*
 * GPIO driver for the ACCES 104-IDIO-16 family
 * Copyright (C) 2015 William Breathitt Gray
 *
 * This driver supports the following ACCES devices: 104-IDIO-16,
 * 104-IDIO-16E, 104-IDO-16, 104-IDIO-8, 104-IDIO-8E, and 104-IDO-8.
 */
#include <linex/bitmap.h>
#include <linex/device.h>
#include <linex/errno.h>
#include <linex/gpio/driver.h>
#include <linex/io.h>
#include <linex/ioport.h>
#include <linex/interrupt.h>
#include <linex/irqdesc.h>
#include <linex/isa.h>
#include <linex/kernel.h>
#include <linex/module.h>
#include <linex/moduleparam.h>
#include <linex/spinlock.h>
#include <linex/types.h>

#include "gpio-idio-16.h"

#define IDIO_16_EXTENT 8
#define MAX_NUM_IDIO_16 max_num_isa_dev(IDIO_16_EXTENT)

static unsigned int base[MAX_NUM_IDIO_16];
static unsigned int num_idio_16;
module_param_hw_array(base, uint, ioport, &num_idio_16, 0);
MODULE_PARM_DESC(base, "ACCES 104-IDIO-16 base addresses");

static unsigned int irq[MAX_NUM_IDIO_16];
static unsigned int num_irq;
module_param_hw_array(irq, uint, irq, &num_irq, 0);
MODULE_PARM_DESC(irq, "ACCES 104-IDIO-16 interrupt line numbers");

/**
 * struct idio_16_gpio - GPIO device private data structure
 * @chip:	instance of the gpio_chip
 * @lock:	synchronization lock to prevent I/O race conditions
 * @irq_mask:	I/O bits affected by interrupts
 * @reg:	I/O address offset for the device registers
 * @state:	ACCES IDIO-16 device state
 */
struct idio_16_gpio {
	struct gpio_chip chip;
	raw_spinlock_t lock;
	unsigned long irq_mask;
	struct idio_16 __iomem *reg;
	struct idio_16_state state;
};

static int idio_16_gpio_get_direction(struct gpio_chip *chip,
				      unsigned int offset)
{
	if (idio_16_get_direction(offset))
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
}

static int idio_16_gpio_direction_input(struct gpio_chip *chip,
					unsigned int offset)
{
	return 0;
}

static int idio_16_gpio_direction_output(struct gpio_chip *chip,
	unsigned int offset, int value)
{
	chip->set(chip, offset, value);
	return 0;
}

static int idio_16_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);

	return idio_16_get(idio16gpio->reg, &idio16gpio->state, offset);
}

static int idio_16_gpio_get_multiple(struct gpio_chip *chip,
	unsigned long *mask, unsigned long *bits)
{
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);

	idio_16_get_multiple(idio16gpio->reg, &idio16gpio->state, mask, bits);

	return 0;
}

static void idio_16_gpio_set(struct gpio_chip *chip, unsigned int offset,
			     int value)
{
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);

	idio_16_set(idio16gpio->reg, &idio16gpio->state, offset, value);
}

static void idio_16_gpio_set_multiple(struct gpio_chip *chip,
	unsigned long *mask, unsigned long *bits)
{
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);

	idio_16_set_multiple(idio16gpio->reg, &idio16gpio->state, mask, bits);
}

static void idio_16_irq_ack(struct irq_data *data)
{
}

static void idio_16_irq_mask(struct irq_data *data)
{
	struct gpio_chip *chip = irq_data_get_irq_chip_data(data);
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);
	const unsigned long offset = irqd_to_hwirq(data);
	unsigned long flags;

	idio16gpio->irq_mask &= ~BIT(offset);
	gpiochip_disable_irq(chip, offset);

	if (!idio16gpio->irq_mask) {
		raw_spin_lock_irqsave(&idio16gpio->lock, flags);

		iowrite8(0, &idio16gpio->reg->irq_ctl);

		raw_spin_unlock_irqrestore(&idio16gpio->lock, flags);
	}
}

static void idio_16_irq_unmask(struct irq_data *data)
{
	struct gpio_chip *chip = irq_data_get_irq_chip_data(data);
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(chip);
	const unsigned long offset = irqd_to_hwirq(data);
	const unsigned long prev_irq_mask = idio16gpio->irq_mask;
	unsigned long flags;

	gpiochip_enable_irq(chip, offset);
	idio16gpio->irq_mask |= BIT(offset);

	if (!prev_irq_mask) {
		raw_spin_lock_irqsave(&idio16gpio->lock, flags);

		ioread8(&idio16gpio->reg->irq_ctl);

		raw_spin_unlock_irqrestore(&idio16gpio->lock, flags);
	}
}

static int idio_16_irq_set_type(struct irq_data *data, unsigned int flow_type)
{
	/* The only valid irq types are none and both-edges */
	if (flow_type != IRQ_TYPE_NONE &&
		(flow_type & IRQ_TYPE_EDGE_BOTH) != IRQ_TYPE_EDGE_BOTH)
		return -EINVAL;

	return 0;
}

static const struct irq_chip idio_16_irqchip = {
	.name = "104-idio-16",
	.irq_ack = idio_16_irq_ack,
	.irq_mask = idio_16_irq_mask,
	.irq_unmask = idio_16_irq_unmask,
	.irq_set_type = idio_16_irq_set_type,
	.flags = IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS,
};

static irqreturn_t idio_16_irq_handler(int irq, void *dev_id)
{
	struct idio_16_gpio *const idio16gpio = dev_id;
	struct gpio_chip *const chip = &idio16gpio->chip;
	int gpio;

	for_each_set_bit(gpio, &idio16gpio->irq_mask, chip->ngpio)
		generic_handle_domain_irq(chip->irq.domain, gpio);

	raw_spin_lock(&idio16gpio->lock);

	iowrite8(0, &idio16gpio->reg->in0_7);

	raw_spin_unlock(&idio16gpio->lock);

	return IRQ_HANDLED;
}

#define IDIO_16_NGPIO 32
static const char *idio_16_names[IDIO_16_NGPIO] = {
	"OUT0", "OUT1", "OUT2", "OUT3", "OUT4", "OUT5", "OUT6", "OUT7",
	"OUT8", "OUT9", "OUT10", "OUT11", "OUT12", "OUT13", "OUT14", "OUT15",
	"IIN0", "IIN1", "IIN2", "IIN3", "IIN4", "IIN5", "IIN6", "IIN7",
	"IIN8", "IIN9", "IIN10", "IIN11", "IIN12", "IIN13", "IIN14", "IIN15"
};

static int idio_16_irq_init_hw(struct gpio_chip *gc)
{
	struct idio_16_gpio *const idio16gpio = gpiochip_get_data(gc);

	/* Disable IRQ by default */
	iowrite8(0, &idio16gpio->reg->irq_ctl);
	iowrite8(0, &idio16gpio->reg->in0_7);

	return 0;
}

static int idio_16_probe(struct device *dev, unsigned int id)
{
	struct idio_16_gpio *idio16gpio;
	const char *const name = dev_name(dev);
	struct gpio_irq_chip *girq;
	int err;

	idio16gpio = devm_kzalloc(dev, sizeof(*idio16gpio), GFP_KERNEL);
	if (!idio16gpio)
		return -ENOMEM;

	if (!devm_request_region(dev, base[id], IDIO_16_EXTENT, name)) {
		dev_err(dev, "Unable to lock port addresses (0x%X-0x%X)\n",
			base[id], base[id] + IDIO_16_EXTENT);
		return -EBUSY;
	}

	idio16gpio->reg = devm_ioport_map(dev, base[id], IDIO_16_EXTENT);
	if (!idio16gpio->reg)
		return -ENOMEM;

	idio16gpio->chip.label = name;
	idio16gpio->chip.parent = dev;
	idio16gpio->chip.owner = THIS_MODULE;
	idio16gpio->chip.base = -1;
	idio16gpio->chip.ngpio = IDIO_16_NGPIO;
	idio16gpio->chip.names = idio_16_names;
	idio16gpio->chip.get_direction = idio_16_gpio_get_direction;
	idio16gpio->chip.direction_input = idio_16_gpio_direction_input;
	idio16gpio->chip.direction_output = idio_16_gpio_direction_output;
	idio16gpio->chip.get = idio_16_gpio_get;
	idio16gpio->chip.get_multiple = idio_16_gpio_get_multiple;
	idio16gpio->chip.set = idio_16_gpio_set;
	idio16gpio->chip.set_multiple = idio_16_gpio_set_multiple;

	idio_16_state_init(&idio16gpio->state);
	/* FET off states are represented by bit values of "1" */
	bitmap_fill(idio16gpio->state.out_state, IDIO_16_NOUT);

	girq = &idio16gpio->chip.irq;
	gpio_irq_chip_set_chip(girq, &idio_16_irqchip);
	/* This will let us handle the parent IRQ in the driver */
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_edge_irq;
	girq->init_hw = idio_16_irq_init_hw;

	raw_spin_lock_init(&idio16gpio->lock);

	err = devm_gpiochip_add_data(dev, &idio16gpio->chip, idio16gpio);
	if (err) {
		dev_err(dev, "GPIO registering failed (%d)\n", err);
		return err;
	}

	err = devm_request_irq(dev, irq[id], idio_16_irq_handler, 0, name,
		idio16gpio);
	if (err) {
		dev_err(dev, "IRQ handler registering failed (%d)\n", err);
		return err;
	}

	return 0;
}

static struct isa_driver idio_16_driver = {
	.probe = idio_16_probe,
	.driver = {
		.name = "104-idio-16"
	},
};

module_isa_driver_with_irq(idio_16_driver, num_idio_16, num_irq);

MODULE_AUTHOR("William Breathitt Gray <vilhelm.gray@gmail.com>");
MODULE_DESCRIPTION("ACCES 104-IDIO-16 GPIO driver");
MODULE_LICENSE("GPL v2");
MODULE_IMPORT_NS(GPIO_IDIO_16);
