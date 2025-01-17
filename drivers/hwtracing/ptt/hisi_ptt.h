/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for HiSilicon PCIe tune and trace device
 *
 * Copyright (c) 2022 HiSilicon Technologies Co., Ltd.
 * Author: Yicong Yang <yangyicong@hisilicon.com>
 */

#ifndef _HISI_PTT_H
#define _HISI_PTT_H

#include <linex/bits.h>
#include <linex/cpumask.h>
#include <linex/list.h>
#include <linex/mutex.h>
#include <linex/pci.h>
#include <linex/perf_event.h>
#include <linex/spinlock.h>
#include <linex/types.h>

#define DRV_NAME "hisi_ptt"

/*
 * The definition of the device registers and register fields.
 */
#define HISI_PTT_TUNING_CTRL		0x0000
#define   HISI_PTT_TUNING_CTRL_CODE	GENMASK(15, 0)
#define   HISI_PTT_TUNING_CTRL_SUB	GENMASK(23, 16)
#define HISI_PTT_TUNING_DATA		0x0004
#define   HISI_PTT_TUNING_DATA_VAL_MASK	GENMASK(15, 0)
#define HISI_PTT_TRACE_ADDR_SIZE	0x0800
#define HISI_PTT_TRACE_ADDR_BASE_LO_0	0x0810
#define HISI_PTT_TRACE_ADDR_BASE_HI_0	0x0814
#define HISI_PTT_TRACE_ADDR_STRIDE	0x8
#define HISI_PTT_TRACE_CTRL		0x0850
#define   HISI_PTT_TRACE_CTRL_EN	BIT(0)
#define   HISI_PTT_TRACE_CTRL_RST	BIT(1)
#define   HISI_PTT_TRACE_CTRL_RXTX_SEL	GENMASK(3, 2)
#define   HISI_PTT_TRACE_CTRL_TYPE_SEL	GENMASK(7, 4)
#define   HISI_PTT_TRACE_CTRL_DATA_FORMAT	BIT(14)
#define   HISI_PTT_TRACE_CTRL_FILTER_MODE	BIT(15)
#define   HISI_PTT_TRACE_CTRL_TARGET_SEL	GENMASK(31, 16)
#define HISI_PTT_TRACE_INT_STAT		0x0890
#define   HISI_PTT_TRACE_INT_STAT_MASK	GENMASK(3, 0)
#define HISI_PTT_TRACE_INT_MASK		0x0894
#define HISI_PTT_TUNING_INT_STAT	0x0898
#define   HISI_PTT_TUNING_INT_STAT_MASK	BIT(0)
#define HISI_PTT_TRACE_WR_STS		0x08a0
#define   HISI_PTT_TRACE_WR_STS_WRITE	GENMASK(27, 0)
#define   HISI_PTT_TRACE_WR_STS_BUFFER	GENMASK(29, 28)
#define HISI_PTT_TRACE_STS		0x08b0
#define   HISI_PTT_TRACE_IDLE		BIT(0)
#define HISI_PTT_DEVICE_RANGE		0x0fe0
#define   HISI_PTT_DEVICE_RANGE_UPPER	GENMASK(31, 16)
#define   HISI_PTT_DEVICE_RANGE_LOWER	GENMASK(15, 0)
#define HISI_PTT_LOCATION		0x0fe8
#define   HISI_PTT_CORE_ID		GENMASK(15, 0)
#define   HISI_PTT_SICL_ID		GENMASK(31, 16)

/* Parameters of PTT trace DMA part. */
#define HISI_PTT_TRACE_DMA_IRQ			0
#define HISI_PTT_TRACE_BUF_CNT			4
#define HISI_PTT_TRACE_BUF_SIZE			SZ_4M
#define HISI_PTT_TRACE_TOTAL_BUF_SIZE		(HISI_PTT_TRACE_BUF_SIZE * \
						 HISI_PTT_TRACE_BUF_CNT)
/* Wait time for hardware DMA to reset */
#define HISI_PTT_RESET_TIMEOUT_US	10UL
#define HISI_PTT_RESET_POLL_INTERVAL_US	1UL
/* Poll timeout and interval for waiting hardware work to finish */
#define HISI_PTT_WAIT_TUNE_TIMEOUT_US	1000000UL
#define HISI_PTT_WAIT_TRACE_TIMEOUT_US	100UL
#define HISI_PTT_WAIT_POLL_INTERVAL_US	10UL

#define HISI_PCIE_CORE_PORT_ID(devfn)	((PCI_SLOT(devfn) & 0x7) << 1)

/* Definition of the PMU configs */
#define HISI_PTT_PMU_FILTER_IS_PORT	BIT(19)
#define HISI_PTT_PMU_FILTER_VAL_MASK	GENMASK(15, 0)
#define HISI_PTT_PMU_DIRECTION_MASK	GENMASK(23, 20)
#define HISI_PTT_PMU_TYPE_MASK		GENMASK(31, 24)
#define HISI_PTT_PMU_FORMAT_MASK	GENMASK(35, 32)

/**
 * struct hisi_ptt_tune_desc - Describe tune event for PTT tune
 * @hisi_ptt:   PTT device this tune event belongs to
 * @name:       name of this event
 * @event_code: code of the event
 */
struct hisi_ptt_tune_desc {
	struct hisi_ptt *hisi_ptt;
	const char *name;
	u32 event_code;
};

/**
 * struct hisi_ptt_dma_buffer - Describe a single trace buffer of PTT trace.
 *                              The detail of the data format is described
 *                              in the documentation of PTT device.
 * @dma:   DMA address of this buffer visible to the device
 * @addr:  virtual address of this buffer visible to the cpu
 */
struct hisi_ptt_dma_buffer {
	dma_addr_t dma;
	void *addr;
};

/**
 * struct hisi_ptt_trace_ctrl - Control and status of PTT trace
 * @trace_buf: array of the trace buffers for holding the trace data.
 *             the length will be HISI_PTT_TRACE_BUF_CNT.
 * @handle:    perf output handle of current trace session
 * @buf_index: the index of current using trace buffer
 * @on_cpu:    current tracing cpu
 * @started:   current trace status, true for started
 * @is_port:   whether we're tracing root port or not
 * @direction: direction of the TLP headers to trace
 * @filter:    filter value for tracing the TLP headers
 * @format:    format of the TLP headers to trace
 * @type:      type of the TLP headers to trace
 */
struct hisi_ptt_trace_ctrl {
	struct hisi_ptt_dma_buffer *trace_buf;
	struct perf_output_handle handle;
	u32 buf_index;
	int on_cpu;
	bool started;
	bool is_port;
	u32 direction:2;
	u32 filter:16;
	u32 format:1;
	u32 type:4;
};

/**
 * struct hisi_ptt_filter_desc - Descriptor of the PTT trace filter
 * @list:    entry of this descriptor in the filter list
 * @is_port: the PCI device of the filter is a Root Port or not
 * @devid:   the PCI device's devid of the filter
 */
struct hisi_ptt_filter_desc {
	struct list_head list;
	bool is_port;
	u16 devid;
};

/**
 * struct hisi_ptt_pmu_buf - Descriptor of the AUX buffer of PTT trace
 * @length:   size of the AUX buffer
 * @nr_pages: number of pages of the AUX buffer
 * @base:     start address of AUX buffer
 * @pos:      position in the AUX buffer to commit traced data
 */
struct hisi_ptt_pmu_buf {
	size_t length;
	int nr_pages;
	void *base;
	long pos;
};

/**
 * struct hisi_ptt - Per PTT device data
 * @trace_ctrl:   the control information of PTT trace
 * @hotplug_node: node for register cpu hotplug event
 * @hisi_ptt_pmu: the pum device of trace
 * @iobase:       base IO address of the device
 * @pdev:         pci_dev of this PTT device
 * @tune_lock:    lock to serialize the tune process
 * @pmu_lock:     lock to serialize the perf process
 * @upper_bdf:    the upper BDF range of the PCI devices managed by this PTT device
 * @lower_bdf:    the lower BDF range of the PCI devices managed by this PTT device
 * @port_filters: the filter list of root ports
 * @req_filters:  the filter list of requester ID
 * @port_mask:    port mask of the managed root ports
 */
struct hisi_ptt {
	struct hisi_ptt_trace_ctrl trace_ctrl;
	struct hlist_node hotplug_node;
	struct pmu hisi_ptt_pmu;
	void __iomem *iobase;
	struct pci_dev *pdev;
	struct mutex tune_lock;
	spinlock_t pmu_lock;
	u32 upper_bdf;
	u32 lower_bdf;

	/*
	 * The trace TLP headers can either be filtered by certain
	 * root port, or by the requester ID. Organize the filters
	 * by @port_filters and @req_filters here. The mask of all
	 * the valid ports is also cached for doing sanity check
	 * of user input.
	 */
	struct list_head port_filters;
	struct list_head req_filters;
	u16 port_mask;
};

#define to_hisi_ptt(pmu) container_of(pmu, struct hisi_ptt, hisi_ptt_pmu)

#endif /* _HISI_PTT_H */
