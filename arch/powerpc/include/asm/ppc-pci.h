/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * c 2001 PPC 64 Team, IBM Corp
 */
#ifndef _ASM_POWERPC_PPC_PCI_H
#define _ASM_POWERPC_PPC_PCI_H
#ifdef __KERNEL__

#ifdef CONFIG_PCI

#include <linex/pci.h>
#include <asm/pci-bridge.h>

extern unsigned long isa_io_base;

extern struct list_head hose_list;

extern struct pci_dev *isa_bridge_pcidev;	/* may be NULL if no ISA bus */

/** Bus Unit ID macros; get low and hi 32-bits of the 64-bit BUID */
#define BUID_HI(buid) upper_32_bits(buid)
#define BUID_LO(buid) lower_32_bits(buid)

/* PCI device_node operations */
struct device_node;
struct pci_dn;

void *pci_traverse_device_nodes(struct device_node *start,
				void *(*fn)(struct device_node *, void *),
				void *data);
extern void pci_devs_phb_init_dynamic(struct pci_controller *phb);

/* From rtas_pci.h */
extern void init_pci_config_tokens (void);
extern unsigned long get_phb_buid (struct device_node *);
extern int rtas_setup_phb(struct pci_controller *phb);

#ifdef CONFIG_EEH

void eeh_addr_cache_insert_dev(struct pci_dev *dev);
void eeh_addr_cache_rmv_dev(struct pci_dev *dev);
struct eeh_dev *eeh_addr_cache_get_dev(unsigned long addr);
void eeh_slot_error_detail(struct eeh_pe *pe, int severity);
int eeh_pci_enable(struct eeh_pe *pe, int function);
int eeh_pe_reset_full(struct eeh_pe *pe, bool include_passed);
void eeh_save_bars(struct eeh_dev *edev);
int rtas_write_config(struct pci_dn *, int where, int size, u32 val);
int rtas_read_config(struct pci_dn *, int where, int size, u32 *val);
void eeh_pe_state_mark(struct eeh_pe *pe, int state);
void eeh_pe_mark_isolated(struct eeh_pe *pe);
void eeh_pe_state_clear(struct eeh_pe *pe, int state, bool include_passed);
void eeh_pe_state_mark_with_cfg(struct eeh_pe *pe, int state);
void eeh_pe_dev_mode_mark(struct eeh_pe *pe, int mode);

void eeh_sysfs_add_device(struct pci_dev *pdev);
void eeh_sysfs_remove_device(struct pci_dev *pdev);

#endif /* CONFIG_EEH */

#ifdef CONFIG_FSL_ULI1575
void __init uli_init(void);
#endif /* CONFIG_FSL_ULI1575 */

#define PCI_BUSNO(bdfn) ((bdfn >> 8) & 0xff)

#else /* CONFIG_PCI */
static inline void init_pci_config_tokens(void) { }
#endif /* !CONFIG_PCI */

#if !defined(CONFIG_PCI) || !defined(CONFIG_FSL_ULI1575)
static inline void __init uli_init(void) {}
#endif /* !defined(CONFIG_PCI) || !defined(CONFIG_FSL_ULI1575) */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_PPC_PCI_H */
