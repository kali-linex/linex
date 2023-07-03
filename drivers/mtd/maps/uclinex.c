/****************************************************************************/

/*
 *	uclinex.c -- generic memory mapped MTD driver for uclinex
 *
 *	(C) Copyright 2002, Greg Ungerer (gerg@snapgear.com)
 *
 *      License: GPL
 */

/****************************************************************************/

#include <linex/moduleparam.h>
#include <linex/types.h>
#include <linex/init.h>
#include <linex/kernel.h>
#include <linex/fs.h>
#include <linex/mm.h>
#include <linex/major.h>
#include <linex/mtd/mtd.h>
#include <linex/mtd/map.h>
#include <linex/mtd/partitions.h>
#include <asm/io.h>
#include <asm/sections.h>

/****************************************************************************/

#ifdef CONFIG_MTD_ROM
#define MAP_NAME "rom"
#else
#define MAP_NAME "ram"
#endif

static struct map_info uclinex_ram_map = {
	.name = MAP_NAME,
	.size = 0,
};

static unsigned long physaddr = -1;
module_param(physaddr, ulong, S_IRUGO);

static struct mtd_info *uclinex_ram_mtdinfo;

/****************************************************************************/

static const struct mtd_partition uclinex_romfs[] = {
	{ .name = "ROMfs" }
};

#define	NUM_PARTITIONS	ARRAY_SIZE(uclinex_romfs)

/****************************************************************************/

static int uclinex_point(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, void **virt, resource_size_t *phys)
{
	struct map_info *map = mtd->priv;
	*virt = map->virt + from;
	if (phys)
		*phys = map->phys + from;
	*retlen = len;
	return(0);
}

/****************************************************************************/

static int __init uclinex_mtd_init(void)
{
	struct mtd_info *mtd;
	struct map_info *mapp;

	mapp = &uclinex_ram_map;

	if (physaddr == -1)
		mapp->phys = (resource_size_t)__bss_stop;
	else
		mapp->phys = physaddr;

	if (!mapp->size)
		mapp->size = PAGE_ALIGN(ntohl(*((unsigned long *)(mapp->phys + 8))));
	mapp->bankwidth = 4;

	printk("uclinex[mtd]: probe address=0x%x size=0x%x\n",
	       	(int) mapp->phys, (int) mapp->size);

	/*
	 * The filesystem is guaranteed to be in direct mapped memory. It is
	 * directly following the kernels own bss region. Following the same
	 * mechanism used by architectures setting up traditional initrds we
	 * use phys_to_virt to get the virtual address of its start.
	 */
	mapp->virt = phys_to_virt(mapp->phys);

	if (mapp->virt == 0) {
		printk("uclinex[mtd]: no virtual mapping?\n");
		return(-EIO);
	}

	simple_map_init(mapp);

	mtd = do_map_probe("map_" MAP_NAME, mapp);
	if (!mtd) {
		printk("uclinex[mtd]: failed to find a mapping?\n");
		return(-ENXIO);
	}

	mtd->owner = THIS_MODULE;
	mtd->_point = uclinex_point;
	mtd->priv = mapp;

	uclinex_ram_mtdinfo = mtd;
	mtd_device_register(mtd, uclinex_romfs, NUM_PARTITIONS);

	return(0);
}
device_initcall(uclinex_mtd_init);

/****************************************************************************/
