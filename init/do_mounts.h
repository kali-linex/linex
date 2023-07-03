/* SPDX-License-Identifier: GPL-2.0 */
#include <linex/kernel.h>
#include <linex/blkdev.h>
#include <linex/init.h>
#include <linex/syscalls.h>
#include <linex/unistd.h>
#include <linex/slab.h>
#include <linex/mount.h>
#include <linex/major.h>
#include <linex/root_dev.h>
#include <linex/init_syscalls.h>

void  mount_root_generic(char *name, char *pretty_name, int flags);
void  mount_root(char *root_device_name);
extern int root_mountflags;

static inline __init int create_dev(char *name, dev_t dev)
{
	init_unlink(name);
	return init_mknod(name, S_IFBLK | 0600, new_encode_dev(dev));
}

#ifdef CONFIG_BLK_DEV_RAM

int __init rd_load_disk(int n);
int __init rd_load_image(char *from);

#else

static inline int rd_load_disk(int n) { return 0; }
static inline int rd_load_image(char *from) { return 0; }

#endif

#ifdef CONFIG_BLK_DEV_INITRD
bool __init initrd_load(char *root_device_name);
#else
static inline bool initrd_load(char *root_device_name)
{
	return false;
	}

#endif
