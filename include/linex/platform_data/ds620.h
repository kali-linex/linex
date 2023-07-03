/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_DS620_H
#define _LINEX_DS620_H

#include <linex/types.h>
#include <linex/i2c.h>

/* platform data for the DS620 temperature sensor and thermostat */

struct ds620_platform_data {
	/*
	 *  Thermostat output pin PO mode:
	 *  0 = always low (default)
	 *  1 = PO_LOW
	 *  2 = PO_HIGH
	 *
	 * (see Documentation/hwmon/ds620.rst)
	 */
	int pomode;
};

#endif /* _LINEX_DS620_H */
