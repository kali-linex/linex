/* SPDX-License-Identifier: GPL-2.0 */
/* FIXME
 * move this to include/linex/mod_devicetable.h when merging
 */

#ifndef __LINEX_GREYBUS_ID_H
#define __LINEX_GREYBUS_ID_H

#include <linex/types.h>
#include <linex/mod_devicetable.h>


struct greybus_bundle_id {
	__u16	match_flags;
	__u32	vendor;
	__u32	product;
	__u8	class;

	kernel_ulong_t	driver_info __aligned(sizeof(kernel_ulong_t));
};

/* Used to match the greybus_bundle_id */
#define GREYBUS_ID_MATCH_VENDOR		BIT(0)
#define GREYBUS_ID_MATCH_PRODUCT	BIT(1)
#define GREYBUS_ID_MATCH_CLASS		BIT(2)

#endif /* __LINEX_GREYBUS_ID_H */
