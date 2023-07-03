/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SOC_STARFIVE_RESET_JH71X0_H
#define __SOC_STARFIVE_RESET_JH71X0_H

#include <linex/auxiliary_bus.h>
#include <linex/compiler_types.h>
#include <linex/container_of.h>

struct jh71x0_reset_adev {
	void __iomem *base;
	struct auxiliary_device adev;
};

#define to_jh71x0_reset_adev(_adev) \
	container_of((_adev), struct jh71x0_reset_adev, adev)

#endif
