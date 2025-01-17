// SPDX-License-Identifier: GPL-2.0-only
/*
 * An I2C driver for the Intersil ISL 12022
 *
 * Author: Roman Fietze <roman.fietze@telemotive.de>
 *
 * Based on the Philips PCF8563 RTC
 * by Alessandro Zummo <a.zummo@towertech.it>.
 */

#include <linex/bcd.h>
#include <linex/err.h>
#include <linex/hwmon.h>
#include <linex/i2c.h>
#include <linex/module.h>
#include <linex/regmap.h>
#include <linex/rtc.h>
#include <linex/slab.h>

#include <asm/byteorder.h>

/* ISL register offsets */
#define ISL12022_REG_SC		0x00
#define ISL12022_REG_MN		0x01
#define ISL12022_REG_HR		0x02
#define ISL12022_REG_DT		0x03
#define ISL12022_REG_MO		0x04
#define ISL12022_REG_YR		0x05
#define ISL12022_REG_DW		0x06

#define ISL12022_REG_SR		0x07
#define ISL12022_REG_INT	0x08

#define ISL12022_REG_BETA	0x0d
#define ISL12022_REG_TEMP_L	0x28

/* ISL register bits */
#define ISL12022_HR_MIL		(1 << 7)	/* military or 24 hour time */

#define ISL12022_SR_LBAT85	(1 << 2)
#define ISL12022_SR_LBAT75	(1 << 1)

#define ISL12022_INT_WRTC	(1 << 6)

#define ISL12022_BETA_TSE	(1 << 7)

static umode_t isl12022_hwmon_is_visible(const void *data,
					 enum hwmon_sensor_types type,
					 u32 attr, int channel)
{
	if (type == hwmon_temp && attr == hwmon_temp_input)
		return 0444;

	return 0;
}

/*
 * A user-initiated temperature conversion is not started by this function,
 * so the temperature is updated once every ~60 seconds.
 */
static int isl12022_hwmon_read_temp(struct device *dev, long *mC)
{
	struct regmap *regmap = dev_get_drvdata(dev);
	int temp, ret;
	__le16 buf;

	ret = regmap_bulk_read(regmap, ISL12022_REG_TEMP_L, &buf, sizeof(buf));
	if (ret)
		return ret;
	/*
	 * Temperature is represented as a 10-bit number, unit half-Kelvins.
	 */
	temp = le16_to_cpu(buf);
	temp *= 500;
	temp -= 273000;

	*mC = temp;

	return 0;
}

static int isl12022_hwmon_read(struct device *dev,
			       enum hwmon_sensor_types type,
			       u32 attr, int channel, long *val)
{
	if (type == hwmon_temp && attr == hwmon_temp_input)
		return isl12022_hwmon_read_temp(dev, val);

	return -EOPNOTSUPP;
}

static const struct hwmon_channel_info *isl12022_hwmon_info[] = {
	HWMON_CHANNEL_INFO(temp, HWMON_T_INPUT),
	NULL
};

static const struct hwmon_ops isl12022_hwmon_ops = {
	.is_visible = isl12022_hwmon_is_visible,
	.read = isl12022_hwmon_read,
};

static const struct hwmon_chip_info isl12022_hwmon_chip_info = {
	.ops = &isl12022_hwmon_ops,
	.info = isl12022_hwmon_info,
};

static void isl12022_hwmon_register(struct device *dev)
{
	struct regmap *regmap = dev_get_drvdata(dev);
	struct device *hwmon;
	int ret;

	if (!IS_REACHABLE(CONFIG_HWMON))
		return;

	ret = regmap_update_bits(regmap, ISL12022_REG_BETA,
				 ISL12022_BETA_TSE, ISL12022_BETA_TSE);
	if (ret) {
		dev_warn(dev, "unable to enable temperature sensor\n");
		return;
	}

	hwmon = devm_hwmon_device_register_with_info(dev, "isl12022", regmap,
						     &isl12022_hwmon_chip_info,
						     NULL);
	if (IS_ERR(hwmon))
		dev_warn(dev, "unable to register hwmon device: %pe\n", hwmon);
}

/*
 * In the routines that deal directly with the isl12022 hardware, we use
 * rtc_time -- month 0-11, hour 0-23, yr = calendar year-epoch.
 */
static int isl12022_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct regmap *regmap = dev_get_drvdata(dev);
	uint8_t buf[ISL12022_REG_INT + 1];
	int ret;

	ret = regmap_bulk_read(regmap, ISL12022_REG_SC, buf, sizeof(buf));
	if (ret)
		return ret;

	if (buf[ISL12022_REG_SR] & (ISL12022_SR_LBAT85 | ISL12022_SR_LBAT75)) {
		dev_warn(dev,
			 "voltage dropped below %u%%, date and time is not reliable.\n",
			 buf[ISL12022_REG_SR] & ISL12022_SR_LBAT85 ? 85 : 75);
	}

	dev_dbg(dev,
		"raw data is sec=%02x, min=%02x, hr=%02x, mday=%02x, mon=%02x, year=%02x, wday=%02x, sr=%02x, int=%02x",
		buf[ISL12022_REG_SC],
		buf[ISL12022_REG_MN],
		buf[ISL12022_REG_HR],
		buf[ISL12022_REG_DT],
		buf[ISL12022_REG_MO],
		buf[ISL12022_REG_YR],
		buf[ISL12022_REG_DW],
		buf[ISL12022_REG_SR],
		buf[ISL12022_REG_INT]);

	tm->tm_sec = bcd2bin(buf[ISL12022_REG_SC] & 0x7F);
	tm->tm_min = bcd2bin(buf[ISL12022_REG_MN] & 0x7F);
	tm->tm_hour = bcd2bin(buf[ISL12022_REG_HR] & 0x3F);
	tm->tm_mday = bcd2bin(buf[ISL12022_REG_DT] & 0x3F);
	tm->tm_wday = buf[ISL12022_REG_DW] & 0x07;
	tm->tm_mon = bcd2bin(buf[ISL12022_REG_MO] & 0x1F) - 1;
	tm->tm_year = bcd2bin(buf[ISL12022_REG_YR]) + 100;

	dev_dbg(dev, "%s: %ptR\n", __func__, tm);

	return 0;
}

static int isl12022_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct regmap *regmap = dev_get_drvdata(dev);
	int ret;
	uint8_t buf[ISL12022_REG_DW + 1];

	dev_dbg(dev, "%s: %ptR\n", __func__, tm);

	/* Ensure the write enable bit is set. */
	ret = regmap_update_bits(regmap, ISL12022_REG_INT,
				 ISL12022_INT_WRTC, ISL12022_INT_WRTC);
	if (ret)
		return ret;

	/* hours, minutes and seconds */
	buf[ISL12022_REG_SC] = bin2bcd(tm->tm_sec);
	buf[ISL12022_REG_MN] = bin2bcd(tm->tm_min);
	buf[ISL12022_REG_HR] = bin2bcd(tm->tm_hour) | ISL12022_HR_MIL;

	buf[ISL12022_REG_DT] = bin2bcd(tm->tm_mday);

	/* month, 1 - 12 */
	buf[ISL12022_REG_MO] = bin2bcd(tm->tm_mon + 1);

	/* year and century */
	buf[ISL12022_REG_YR] = bin2bcd(tm->tm_year % 100);

	buf[ISL12022_REG_DW] = tm->tm_wday & 0x07;

	return regmap_bulk_write(regmap, ISL12022_REG_SC, buf, sizeof(buf));
}

static const struct rtc_class_ops isl12022_rtc_ops = {
	.read_time	= isl12022_rtc_read_time,
	.set_time	= isl12022_rtc_set_time,
};

static const struct regmap_config regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.use_single_write = true,
};

static int isl12022_probe(struct i2c_client *client)
{
	struct rtc_device *rtc;
	struct regmap *regmap;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -ENODEV;

	regmap = devm_regmap_init_i2c(client, &regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(&client->dev, "regmap allocation failed\n");
		return PTR_ERR(regmap);
	}

	dev_set_drvdata(&client->dev, regmap);

	isl12022_hwmon_register(&client->dev);

	rtc = devm_rtc_allocate_device(&client->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	rtc->ops = &isl12022_rtc_ops;
	rtc->range_min = RTC_TIMESTAMP_BEGIN_2000;
	rtc->range_max = RTC_TIMESTAMP_END_2099;

	return devm_rtc_register_device(rtc);
}

static const struct of_device_id isl12022_dt_match[] = {
	{ .compatible = "isl,isl12022" }, /* for backward compat., don't use */
	{ .compatible = "isil,isl12022" },
	{ },
};
MODULE_DEVICE_TABLE(of, isl12022_dt_match);

static const struct i2c_device_id isl12022_id[] = {
	{ "isl12022", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, isl12022_id);

static struct i2c_driver isl12022_driver = {
	.driver		= {
		.name	= "rtc-isl12022",
		.of_match_table = isl12022_dt_match,
	},
	.probe_new	= isl12022_probe,
	.id_table	= isl12022_id,
};

module_i2c_driver(isl12022_driver);

MODULE_AUTHOR("roman.fietze@telemotive.de");
MODULE_DESCRIPTION("ISL 12022 RTC driver");
MODULE_LICENSE("GPL");
