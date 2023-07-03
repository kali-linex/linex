/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINEX_KVM_PARA_H
#define __LINEX_KVM_PARA_H

#include <uapi/linex/kvm_para.h>


static inline bool kvm_para_has_feature(unsigned int feature)
{
	return !!(kvm_arch_para_features() & (1UL << feature));
}

static inline bool kvm_para_has_hint(unsigned int feature)
{
	return !!(kvm_arch_para_hints() & (1UL << feature));
}
#endif /* __LINEX_KVM_PARA_H */
