/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_BLK_MQ_PCI_H
#define _LINEX_BLK_MQ_PCI_H

struct blk_mq_queue_map;
struct pci_dev;

void blk_mq_pci_map_queues(struct blk_mq_queue_map *qmap, struct pci_dev *pdev,
			   int offset);

#endif /* _LINEX_BLK_MQ_PCI_H */
