/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linex kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __XEN_BLKIF__BACKEND__COMMON_H__
#define __XEN_BLKIF__BACKEND__COMMON_H__

#include <linex/module.h>
#include <linex/interrupt.h>
#include <linex/slab.h>
#include <linex/blkdev.h>
#include <linex/vmalloc.h>
#include <linex/wait.h>
#include <linex/io.h>
#include <linex/rbtree.h>
#include <asm/setup.h>
#include <asm/hypervisor.h>
#include <xen/grant_table.h>
#include <xen/page.h>
#include <xen/xenbus.h>
#include <xen/interface/io/ring.h>
#include <xen/interface/io/blkif.h>
#include <xen/interface/io/protocols.h>

extern unsigned int xen_blkif_max_ring_order;
extern unsigned int xenblk_max_queues;
/*
 * This is the maximum number of segments that would be allowed in indirect
 * requests. This value will also be passed to the frontend.
 */
#define MAX_INDIRECT_SEGMENTS 256

/*
 * Xen use 4K pages. The guest may use different page size (4K or 64K)
 * Number of Xen pages per segment
 */
#define XEN_PAGES_PER_SEGMENT   (PAGE_SIZE / XEN_PAGE_SIZE)

#define XEN_PAGES_PER_INDIRECT_FRAME \
	(XEN_PAGE_SIZE/sizeof(struct blkif_request_segment))
#define SEGS_PER_INDIRECT_FRAME	\
	(XEN_PAGES_PER_INDIRECT_FRAME / XEN_PAGES_PER_SEGMENT)

#define MAX_INDIRECT_PAGES \
	((MAX_INDIRECT_SEGMENTS + SEGS_PER_INDIRECT_FRAME - 1)/SEGS_PER_INDIRECT_FRAME)
#define INDIRECT_PAGES(_segs) DIV_ROUND_UP(_segs, XEN_PAGES_PER_INDIRECT_FRAME)

/* Not a real protocol.  Used to generate ring structs which contain
 * the elements common to all protocols only.  This way we get a
 * compiler-checkable way to use common struct elements, so we can
 * avoid using switch(protocol) in a number of places.  */
struct blkif_common_request {
	char dummy;
};

/* i386 protocol version */

struct blkif_x86_32_request_rw {
	uint8_t        nr_segments;  /* number of segments                   */
	blkif_vdev_t   handle;       /* only for read/write requests         */
	uint64_t       id;           /* private guest value, echoed in resp  */
	blkif_sector_t sector_number;/* start sector idx on disk (r/w only)  */
	struct blkif_request_segment seg[BLKIF_MAX_SEGMENTS_PER_REQUEST];
} __attribute__((__packed__));

struct blkif_x86_32_request_discard {
	uint8_t        flag;         /* BLKIF_DISCARD_SECURE or zero         */
	blkif_vdev_t   _pad1;        /* was "handle" for read/write requests */
	uint64_t       id;           /* private guest value, echoed in resp  */
	blkif_sector_t sector_number;/* start sector idx on disk (r/w only)  */
	uint64_t       nr_sectors;
} __attribute__((__packed__));

struct blkif_x86_32_request_other {
	uint8_t        _pad1;
	blkif_vdev_t   _pad2;
	uint64_t       id;           /* private guest value, echoed in resp  */
} __attribute__((__packed__));

struct blkif_x86_32_request_indirect {
	uint8_t        indirect_op;
	uint16_t       nr_segments;
	uint64_t       id;
	blkif_sector_t sector_number;
	blkif_vdev_t   handle;
	uint16_t       _pad1;
	grant_ref_t    indirect_grefs[BLKIF_MAX_INDIRECT_PAGES_PER_REQUEST];
	/*
	 * The maximum number of indirect segments (and pages) that will
	 * be used is determined by MAX_INDIRECT_SEGMENTS, this value
	 * is also exported to the guest (via xenstore
	 * feature-max-indirect-segments entry), so the frontend knows how
	 * many indirect segments the backend supports.
	 */
	uint64_t       _pad2;        /* make it 64 byte aligned */
} __attribute__((__packed__));

struct blkif_x86_32_request {
	uint8_t        operation;    /* BLKIF_OP_???                         */
	union {
		struct blkif_x86_32_request_rw rw;
		struct blkif_x86_32_request_discard discard;
		struct blkif_x86_32_request_other other;
		struct blkif_x86_32_request_indirect indirect;
	} u;
} __attribute__((__packed__));

/* x86_64 protocol version */

struct blkif_x86_64_request_rw {
	uint8_t        nr_segments;  /* number of segments                   */
	blkif_vdev_t   handle;       /* only for read/write requests         */
	uint32_t       _pad1;        /* offsetof(blkif_reqest..,u.rw.id)==8  */
	uint64_t       id;
	blkif_sector_t sector_number;/* start sector idx on disk (r/w only)  */
	struct blkif_request_segment seg[BLKIF_MAX_SEGMENTS_PER_REQUEST];
} __attribute__((__packed__));

struct blkif_x86_64_request_discard {
	uint8_t        flag;         /* BLKIF_DISCARD_SECURE or zero         */
	blkif_vdev_t   _pad1;        /* was "handle" for read/write requests */
        uint32_t       _pad2;        /* offsetof(blkif_..,u.discard.id)==8   */
	uint64_t       id;
	blkif_sector_t sector_number;/* start sector idx on disk (r/w only)  */
	uint64_t       nr_sectors;
} __attribute__((__packed__));

struct blkif_x86_64_request_other {
	uint8_t        _pad1;
	blkif_vdev_t   _pad2;
	uint32_t       _pad3;        /* offsetof(blkif_..,u.discard.id)==8   */
	uint64_t       id;           /* private guest value, echoed in resp  */
} __attribute__((__packed__));

struct blkif_x86_64_request_indirect {
	uint8_t        indirect_op;
	uint16_t       nr_segments;
	uint32_t       _pad1;        /* offsetof(blkif_..,u.indirect.id)==8   */
	uint64_t       id;
	blkif_sector_t sector_number;
	blkif_vdev_t   handle;
	uint16_t       _pad2;
	grant_ref_t    indirect_grefs[BLKIF_MAX_INDIRECT_PAGES_PER_REQUEST];
	/*
	 * The maximum number of indirect segments (and pages) that will
	 * be used is determined by MAX_INDIRECT_SEGMENTS, this value
	 * is also exported to the guest (via xenstore
	 * feature-max-indirect-segments entry), so the frontend knows how
	 * many indirect segments the backend supports.
	 */
	uint32_t       _pad3;        /* make it 64 byte aligned */
} __attribute__((__packed__));

struct blkif_x86_64_request {
	uint8_t        operation;    /* BLKIF_OP_???                         */
	union {
		struct blkif_x86_64_request_rw rw;
		struct blkif_x86_64_request_discard discard;
		struct blkif_x86_64_request_other other;
		struct blkif_x86_64_request_indirect indirect;
	} u;
} __attribute__((__packed__));

DEFINE_RING_TYPES(blkif_common, struct blkif_common_request,
		  struct blkif_response);
DEFINE_RING_TYPES(blkif_x86_32, struct blkif_x86_32_request,
		  struct blkif_response __packed);
DEFINE_RING_TYPES(blkif_x86_64, struct blkif_x86_64_request,
		  struct blkif_response);

union blkif_back_rings {
	struct blkif_back_ring        native;
	struct blkif_common_back_ring common;
	struct blkif_x86_32_back_ring x86_32;
	struct blkif_x86_64_back_ring x86_64;
};

enum blkif_protocol {
	BLKIF_PROTOCOL_NATIVE = 1,
	BLKIF_PROTOCOL_X86_32 = 2,
	BLKIF_PROTOCOL_X86_64 = 3,
};

/*
 * Default protocol if the frontend doesn't specify one.
 */
#ifdef CONFIG_X86
#  define BLKIF_PROTOCOL_DEFAULT BLKIF_PROTOCOL_X86_32
#else
#  define BLKIF_PROTOCOL_DEFAULT BLKIF_PROTOCOL_NATIVE
#endif

struct xen_vbd {
	/* What the domain refers to this vbd as. */
	blkif_vdev_t		handle;
	/* Non-zero -> read-only */
	unsigned char		readonly;
	/* VDISK_xxx */
	unsigned char		type;
	/* phys device that this vbd maps to. */
	u32			pdevice;
	struct block_device	*bdev;
	/* Cached size parameter. */
	sector_t		size;
	unsigned int		flush_support:1;
	unsigned int		discard_secure:1;
	/* Connect-time cached feature_persistent parameter value */
	unsigned int		feature_gnt_persistent_parm:1;
	/* Persistent grants feature negotiation result */
	unsigned int		feature_gnt_persistent:1;
	unsigned int		overflow_max_grants:1;
};

struct backend_info;

/* Number of requests that we can fit in a ring */
#define XEN_BLKIF_REQS_PER_PAGE		32

struct persistent_gnt {
	struct page *page;
	grant_ref_t gnt;
	grant_handle_t handle;
	unsigned long last_used;
	bool active;
	struct rb_node node;
	struct list_head remove_node;
};

/* Per-ring information. */
struct xen_blkif_ring {
	/* Physical parameters of the comms window. */
	unsigned int		irq;
	union blkif_back_rings	blk_rings;
	void			*blk_ring;
	/* Private fields. */
	spinlock_t		blk_ring_lock;

	wait_queue_head_t	wq;
	atomic_t		inflight;
	bool			active;
	/* One thread per blkif ring. */
	struct task_struct	*xenblkd;
	unsigned int		waiting_reqs;

	/* List of all 'pending_req' available */
	struct list_head	pending_free;
	/* And its spinlock. */
	spinlock_t		pending_free_lock;
	wait_queue_head_t	pending_free_wq;

	/* Tree to store persistent grants. */
	struct rb_root		persistent_gnts;
	unsigned int		persistent_gnt_c;
	atomic_t		persistent_gnt_in_use;
	unsigned long           next_lru;

	/* Statistics. */
	unsigned long		st_print;
	unsigned long long	st_rd_req;
	unsigned long long	st_wr_req;
	unsigned long long	st_oo_req;
	unsigned long long	st_f_req;
	unsigned long long	st_ds_req;
	unsigned long long	st_rd_sect;
	unsigned long long	st_wr_sect;

	/* Used by the kworker that offload work from the persistent purge. */
	struct list_head	persistent_purge_list;
	struct work_struct	persistent_purge_work;

	/* Buffer of free pages to map grant refs. */
	struct gnttab_page_cache free_pages;

	struct work_struct	free_work;
	/* Thread shutdown wait queue. */
	wait_queue_head_t	shutdown_wq;
	struct xen_blkif	*blkif;
};

struct xen_blkif {
	/* Unique identifier for this interface. */
	domid_t			domid;
	unsigned int		handle;
	/* Comms information. */
	enum blkif_protocol	blk_protocol;
	/* The VBD attached to this interface. */
	struct xen_vbd		vbd;
	/* Back pointer to the backend_info. */
	struct backend_info	*be;
	atomic_t		refcnt;
	/* for barrier (drain) requests */
	struct completion	drain_complete;
	atomic_t		drain;

	struct work_struct	free_work;
	unsigned int		nr_ring_pages;
	bool			multi_ref;
	/* All rings for this device. */
	struct xen_blkif_ring	*rings;
	unsigned int		nr_rings;
	unsigned long		buffer_squeeze_end;
};

struct seg_buf {
	unsigned long offset;
	unsigned int nsec;
};

struct grant_page {
	struct page		*page;
	struct persistent_gnt	*persistent_gnt;
	grant_handle_t		handle;
	grant_ref_t		gref;
};

/*
 * Each outstanding request that we've passed to the lower device layers has a
 * 'pending_req' allocated to it. Each buffer_head that completes decrements
 * the pendcnt towards zero. When it hits zero, the specified domain has a
 * response queued for it, with the saved 'id' passed back.
 */
struct pending_req {
	struct xen_blkif_ring   *ring;
	u64			id;
	int			nr_segs;
	atomic_t		pendcnt;
	unsigned short		operation;
	int			status;
	struct list_head	free_list;
	struct grant_page	*segments[MAX_INDIRECT_SEGMENTS];
	/* Indirect descriptors */
	struct grant_page	*indirect_pages[MAX_INDIRECT_PAGES];
	struct seg_buf		seg[MAX_INDIRECT_SEGMENTS];
	struct bio		*biolist[MAX_INDIRECT_SEGMENTS];
	struct gnttab_unmap_grant_ref unmap[MAX_INDIRECT_SEGMENTS];
	struct page                   *unmap_pages[MAX_INDIRECT_SEGMENTS];
	struct gntab_unmap_queue_data gnttab_unmap_data;
};


#define vbd_sz(_v)	bdev_nr_sectors((_v)->bdev)

#define xen_blkif_get(_b) (atomic_inc(&(_b)->refcnt))
#define xen_blkif_put(_b)				\
	do {						\
		if (atomic_dec_and_test(&(_b)->refcnt))	\
			schedule_work(&(_b)->free_work);\
	} while (0)

struct phys_req {
	unsigned short		dev;
	blkif_sector_t		nr_sects;
	struct block_device	*bdev;
	blkif_sector_t		sector_number;
};

int xen_blkif_interface_init(void);
void xen_blkif_interface_fini(void);

int xen_blkif_xenbus_init(void);
void xen_blkif_xenbus_fini(void);

irqreturn_t xen_blkif_be_int(int irq, void *dev_id);
int xen_blkif_schedule(void *arg);
void xen_blkbk_free_caches(struct xen_blkif_ring *ring);

int xen_blkbk_flush_diskcache(struct xenbus_transaction xbt,
			      struct backend_info *be, int state);

int xen_blkbk_barrier(struct xenbus_transaction xbt,
		      struct backend_info *be, int state);
struct xenbus_device *xen_blkbk_xenbus(struct backend_info *be);
void xen_blkbk_unmap_purged_grants(struct work_struct *work);

#endif /* __XEN_BLKIF__BACKEND__COMMON_H__ */
