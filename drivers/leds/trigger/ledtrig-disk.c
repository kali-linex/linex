// SPDX-License-Identifier: GPL-2.0-only
/*
 * LED Disk Activity Trigger
 *
 * Copyright 2006 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 */

#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/leds.h>

#define BLINK_DELAY 30

DEFINE_LED_TRIGGER(ledtrig_disk);
DEFINE_LED_TRIGGER(ledtrig_disk_read);
DEFINE_LED_TRIGGER(ledtrig_disk_write);

void ledtrig_disk_activity(bool write)
{
	unsigned long blink_delay = BLINK_DELAY;

	led_trigger_blink_oneshot(ledtrig_disk,
				  &blink_delay, &blink_delay, 0);
	if (write)
		led_trigger_blink_oneshot(ledtrig_disk_write,
					  &blink_delay, &blink_delay, 0);
	else
		led_trigger_blink_oneshot(ledtrig_disk_read,
					  &blink_delay, &blink_delay, 0);
}
EXPORT_SYMBOL(ledtrig_disk_activity);

static int __init ledtrig_disk_init(void)
{
	led_trigger_register_simple("disk-activity", &ledtrig_disk);
	led_trigger_register_simple("disk-read", &ledtrig_disk_read);
	led_trigger_register_simple("disk-write", &ledtrig_disk_write);

	return 0;
}
device_initcall(ledtrig_disk_init);
