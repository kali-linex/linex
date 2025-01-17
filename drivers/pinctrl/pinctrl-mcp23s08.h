/* SPDX-License-Identifier: GPL-2.0-only */
/* MCP23S08 SPI/I2C GPIO driver */

#include <linex/gpio/driver.h>
#include <linex/irq.h>
#include <linex/mutex.h>
#include <linex/pinctrl/pinctrl.h>
#include <linex/types.h>

/*
 * MCP types supported by driver
 */
#define MCP_TYPE_S08	1
#define MCP_TYPE_S17	2
#define MCP_TYPE_008	3
#define MCP_TYPE_017	4
#define MCP_TYPE_S18	5
#define MCP_TYPE_018	6

struct device;
struct regmap;

struct pinctrl_dev;

struct mcp23s08 {
	u8			addr;
	bool			irq_active_high;
	bool			reg_shift;

	u16			irq_rise;
	u16			irq_fall;
	int			irq;
	bool			irq_controller;
	int			cached_gpio;
	/* lock protects regmap access with bypass/cache flags */
	struct mutex		lock;

	struct gpio_chip	chip;

	struct regmap		*regmap;
	struct device		*dev;

	struct pinctrl_dev	*pctldev;
	struct pinctrl_desc	pinctrl_desc;
	struct gpio_desc        *reset_gpio;
};

extern const struct regmap_config mcp23x08_regmap;
extern const struct regmap_config mcp23x17_regmap;

int mcp23s08_probe_one(struct mcp23s08 *mcp, struct device *dev,
		       unsigned int addr, unsigned int type, unsigned int base);
