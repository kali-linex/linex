// SPDX-License-Identifier: GPL-2.0
/*
 * PWM Greybus driver.
 *
 * Copyright 2014 Google Inc.
 * Copyright 2014 Linaro Ltd.
 */

#include <linex/kernel.h>
#include <linex/module.h>
#include <linex/slab.h>
#include <linex/pwm.h>
#include <linex/greybus.h>

#include "gbphy.h"

struct gb_pwm_chip {
	struct gb_connection	*connection;
	u8			pwm_max;	/* max pwm number */

	struct pwm_chip		chip;
	struct pwm_chip		*pwm;
};

static inline struct gb_pwm_chip *pwm_chip_to_gb_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct gb_pwm_chip, chip);
}

static int gb_pwm_count_operation(struct gb_pwm_chip *pwmc)
{
	struct gb_pwm_count_response response;
	int ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_PWM_COUNT,
				NULL, 0, &response, sizeof(response));
	if (ret)
		return ret;
	pwmc->pwm_max = response.count;
	return 0;
}

static int gb_pwm_activate_operation(struct gb_pwm_chip *pwmc,
				     u8 which)
{
	struct gb_pwm_activate_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_ACTIVATE,
				&request, sizeof(request), NULL, 0);

	gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_deactivate_operation(struct gb_pwm_chip *pwmc,
				       u8 which)
{
	struct gb_pwm_deactivate_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_DEACTIVATE,
				&request, sizeof(request), NULL, 0);

	gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_config_operation(struct gb_pwm_chip *pwmc,
				   u8 which, u32 duty, u32 period)
{
	struct gb_pwm_config_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;
	request.duty = cpu_to_le32(duty);
	request.period = cpu_to_le32(period);

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_CONFIG,
				&request, sizeof(request), NULL, 0);

	gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_set_polarity_operation(struct gb_pwm_chip *pwmc,
					 u8 which, u8 polarity)
{
	struct gb_pwm_polarity_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;
	request.polarity = polarity;

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_POLARITY,
				&request, sizeof(request), NULL, 0);

	gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_enable_operation(struct gb_pwm_chip *pwmc,
				   u8 which)
{
	struct gb_pwm_enable_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_ENABLE,
				&request, sizeof(request), NULL, 0);
	if (ret)
		gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_disable_operation(struct gb_pwm_chip *pwmc,
				    u8 which)
{
	struct gb_pwm_disable_request request;
	struct gbphy_device *gbphy_dev;
	int ret;

	if (which > pwmc->pwm_max)
		return -EINVAL;

	request.which = which;

	ret = gb_operation_sync(pwmc->connection, GB_PWM_TYPE_DISABLE,
				&request, sizeof(request), NULL, 0);

	gbphy_dev = to_gbphy_dev(pwmc->chip.dev);
	gbphy_runtime_put_autosuspend(gbphy_dev);

	return ret;
}

static int gb_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct gb_pwm_chip *pwmc = pwm_chip_to_gb_pwm_chip(chip);

	return gb_pwm_activate_operation(pwmc, pwm->hwpwm);
};

static void gb_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct gb_pwm_chip *pwmc = pwm_chip_to_gb_pwm_chip(chip);

	if (pwm_is_enabled(pwm))
		dev_warn(chip->dev, "freeing PWM device without disabling\n");

	gb_pwm_deactivate_operation(pwmc, pwm->hwpwm);
}

static int gb_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			const struct pwm_state *state)
{
	int err;
	bool enabled = pwm->state.enabled;
	u64 period = state->period;
	u64 duty_cycle = state->duty_cycle;
	struct gb_pwm_chip *pwmc = pwm_chip_to_gb_pwm_chip(chip);

	/* Set polarity */
	if (state->polarity != pwm->state.polarity) {
		if (enabled) {
			gb_pwm_disable_operation(pwmc, pwm->hwpwm);
			enabled = false;
		}
		err = gb_pwm_set_polarity_operation(pwmc, pwm->hwpwm, state->polarity);
		if (err)
			return err;
	}

	if (!state->enabled) {
		if (enabled)
			gb_pwm_disable_operation(pwmc, pwm->hwpwm);
		return 0;
	}

	/*
	 * Set period and duty cycle
	 *
	 * PWM privodes 64-bit period and duty_cycle, but greybus only accepts
	 * 32-bit, so their values have to be limited to U32_MAX.
	 */
	if (period > U32_MAX)
		period = U32_MAX;

	if (duty_cycle > period)
		duty_cycle = period;

	err = gb_pwm_config_operation(pwmc, pwm->hwpwm, duty_cycle, period);
	if (err)
		return err;

	/* enable/disable */
	if (!enabled)
		return gb_pwm_enable_operation(pwmc, pwm->hwpwm);

	return 0;
}

static const struct pwm_ops gb_pwm_ops = {
	.request = gb_pwm_request,
	.free = gb_pwm_free,
	.apply = gb_pwm_apply,
	.owner = THIS_MODULE,
};

static int gb_pwm_probe(struct gbphy_device *gbphy_dev,
			const struct gbphy_device_id *id)
{
	struct gb_connection *connection;
	struct gb_pwm_chip *pwmc;
	struct pwm_chip *pwm;
	int ret;

	pwmc = kzalloc(sizeof(*pwmc), GFP_KERNEL);
	if (!pwmc)
		return -ENOMEM;

	connection = gb_connection_create(gbphy_dev->bundle,
					  le16_to_cpu(gbphy_dev->cport_desc->id),
					  NULL);
	if (IS_ERR(connection)) {
		ret = PTR_ERR(connection);
		goto exit_pwmc_free;
	}

	pwmc->connection = connection;
	gb_connection_set_data(connection, pwmc);
	gb_gbphy_set_data(gbphy_dev, pwmc);

	ret = gb_connection_enable(connection);
	if (ret)
		goto exit_connection_destroy;

	/* Query number of pwms present */
	ret = gb_pwm_count_operation(pwmc);
	if (ret)
		goto exit_connection_disable;

	pwm = &pwmc->chip;

	pwm->dev = &gbphy_dev->dev;
	pwm->ops = &gb_pwm_ops;
	pwm->npwm = pwmc->pwm_max + 1;

	ret = pwmchip_add(pwm);
	if (ret) {
		dev_err(&gbphy_dev->dev,
			"failed to register PWM: %d\n", ret);
		goto exit_connection_disable;
	}

	gbphy_runtime_put_autosuspend(gbphy_dev);
	return 0;

exit_connection_disable:
	gb_connection_disable(connection);
exit_connection_destroy:
	gb_connection_destroy(connection);
exit_pwmc_free:
	kfree(pwmc);
	return ret;
}

static void gb_pwm_remove(struct gbphy_device *gbphy_dev)
{
	struct gb_pwm_chip *pwmc = gb_gbphy_get_data(gbphy_dev);
	struct gb_connection *connection = pwmc->connection;
	int ret;

	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		gbphy_runtime_get_noresume(gbphy_dev);

	pwmchip_remove(&pwmc->chip);
	gb_connection_disable(connection);
	gb_connection_destroy(connection);
	kfree(pwmc);
}

static const struct gbphy_device_id gb_pwm_id_table[] = {
	{ GBPHY_PROTOCOL(GREYBUS_PROTOCOL_PWM) },
	{ },
};
MODULE_DEVICE_TABLE(gbphy, gb_pwm_id_table);

static struct gbphy_driver pwm_driver = {
	.name		= "pwm",
	.probe		= gb_pwm_probe,
	.remove		= gb_pwm_remove,
	.id_table	= gb_pwm_id_table,
};

module_gbphy_driver(pwm_driver);
MODULE_LICENSE("GPL v2");
