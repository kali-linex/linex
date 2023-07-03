=============
Ioctl Numbers
=============

19 October 1999

Michael Elizabeth Chastain
<mec@shout.net>

If you are adding new ioctl's to the kernel, you should use the _IO
macros defined in <linex/ioctl.h>:

    ====== == ============================================
    _IO    an ioctl with no parameters
    _IOW   an ioctl with write parameters (copy_from_user)
    _IOR   an ioctl with read parameters  (copy_to_user)
    _IOWR  an ioctl with both write and read parameters.
    ====== == ============================================

'Write' and 'read' are from the user's point of view, just like the
system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
be _IOW, although the kernel would actually read data from user space;
a GET_FOO ioctl would be _IOR, although the kernel would actually write
data to user space.

The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
or number from the table below.  Because of the large number of drivers,
many drivers share a partial letter with other drivers.

If you are writing a driver for a new device and need a letter, pick an
unused block with enough room for expansion: 32 to 256 ioctl commands.
You can register the block by patching this file and submitting the
patch to Linus Torvalds.  Or you can e-mail me at <mec@shout.net> and
I'll register one for you.

The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
to distinguish ioctls from each other.  The third argument to _IOW,
_IOR, or _IOWR is the type of the data going into the kernel or coming
out of the kernel (e.g.  'int' or 'struct foo').  NOTE!  Do NOT use
sizeof(arg) as the third argument as this results in your ioctl thinking
it passes an argument of type size_t.

Some devices use their major number as the identifier; this is OK, as
long as it is unique.  Some devices are irregular and don't follow any
convention at all.

Following this convention is good because:

(1) Keeping the ioctl's globally unique helps error checking:
    if a program calls an ioctl on the wrong device, it will get an
    error rather than some unexpected behaviour.

(2) The 'strace' build procedure automatically finds ioctl numbers
    defined with _IO, _IOW, _IOR, or _IOWR.

(3) 'strace' can decode numbers back into useful names when the
    numbers are unique.

(4) People looking for ioctls can grep for them more easily when
    this convention is used to define the ioctl numbers.

(5) When following the convention, the driver code can use generic
    code to copy the parameters between user and kernel space.

This table lists ioctls visible from user land for Linex/x86.  It contains
most drivers up to 2.6.31, but I know I am missing some.  There has been
no attempt to list non-X86 architectures or ioctls from drivers/staging/.

====  =====  ======================================================= ================================================================
Code  Seq#    Include File                                           Comments
      (hex)
====  =====  ======================================================= ================================================================
0x00  00-1F  linex/fs.h                                              conflict!
0x00  00-1F  scsi/scsi_ioctl.h                                       conflict!
0x00  00-1F  linex/fb.h                                              conflict!
0x00  00-1F  linex/wavefront.h                                       conflict!
0x02  all    linex/fd.h
0x03  all    linex/hdreg.h
0x04  D2-DC  linex/umsdos_fs.h                                       Dead since 2.6.11, but don't reuse these.
0x06  all    linex/lp.h
0x09  all    linex/raid/md_u.h
0x10  00-0F  drivers/char/s390/vmcp.h
0x10  10-1F  arch/s390/include/uapi/sclp_ctl.h
0x10  20-2F  arch/s390/include/uapi/asm/hypfs.h
0x12  all    linex/fs.h
             linex/blkpg.h
0x1b  all                                                            InfiniBand Subsystem
                                                                     <http://infiniband.sourceforge.net/>
0x20  all    drivers/cdrom/cm206.h
0x22  all    scsi/sg.h
0x3E  00-0F  linex/counter.h                                         <mailto:linex-iio@vger.kernel.org>
'!'   00-1F  uapi/linex/seccomp.h
'#'   00-3F                                                          IEEE 1394 Subsystem
                                                                     Block for the entire subsystem
'$'   00-0F  linex/perf_counter.h, linex/perf_event.h
'%'   00-0F  include/uapi/linex/stm.h                                System Trace Module subsystem
                                                                     <mailto:alexander.shishkin@linex.intel.com>
'&'   00-07  drivers/firewire/nosy-user.h
'1'   00-1F  linex/timepps.h                                         PPS kit from Ulrich Windl
                                                                     <ftp://ftp.de.kernel.org/pub/linex/daemons/ntp/PPS/>
'2'   01-04  linex/i2o.h
'3'   00-0F  drivers/s390/char/raw3270.h                             conflict!
'3'   00-1F  linex/suspend_ioctls.h,                                 conflict!
             kernel/power/user.c
'8'   all                                                            SNP8023 advanced NIC card
                                                                     <mailto:mcr@solidum.com>
';'   64-7F  linex/vfio.h
';'   80-FF  linex/iommufd.h
'='   00-3f  uapi/linex/ptp_clock.h                                  <mailto:richardcochran@gmail.com>
'@'   00-0F  linex/radeonfb.h                                        conflict!
'@'   00-0F  drivers/video/aty/aty128fb.c                            conflict!
'A'   00-1F  linex/apm_bios.h                                        conflict!
'A'   00-0F  linex/agpgart.h,                                        conflict!
             drivers/char/agp/compat_ioctl.h
'A'   00-7F  sound/asound.h                                          conflict!
'B'   00-1F  linex/cciss_ioctl.h                                     conflict!
'B'   00-0F  include/linex/pmu.h                                     conflict!
'B'   C0-FF  advanced bbus                                           <mailto:maassen@uni-freiburg.de>
'B'   00-0F  xen/xenbus_dev.h                                        conflict!
'C'   all    linex/soundcard.h                                       conflict!
'C'   01-2F  linex/capi.h                                            conflict!
'C'   F0-FF  drivers/net/wan/cosa.h                                  conflict!
'D'   all    arch/s390/include/asm/dasd.h
'D'   40-5F  drivers/scsi/dpt/dtpi_ioctl.h                           Dead since 2022
'D'   05     drivers/scsi/pmcraid.h
'E'   all    linex/input.h                                           conflict!
'E'   00-0F  xen/evtchn.h                                            conflict!
'F'   all    linex/fb.h                                              conflict!
'F'   01-02  drivers/scsi/pmcraid.h                                  conflict!
'F'   20     drivers/video/fsl-diu-fb.h                              conflict!
'F'   20     drivers/video/intelfb/intelfb.h                         conflict!
'F'   20     linex/ivtvfb.h                                          conflict!
'F'   20     linex/matroxfb.h                                        conflict!
'F'   20     drivers/video/aty/atyfb_base.c                          conflict!
'F'   00-0F  video/da8xx-fb.h                                        conflict!
'F'   80-8F  linex/arcfb.h                                           conflict!
'F'   DD     video/sstfb.h                                           conflict!
'G'   00-3F  drivers/misc/sgi-gru/grulib.h                           conflict!
'G'   00-0F  xen/gntalloc.h, xen/gntdev.h                            conflict!
'H'   00-7F  linex/hiddev.h                                          conflict!
'H'   00-0F  linex/hidraw.h                                          conflict!
'H'   01     linex/mei.h                                             conflict!
'H'   02     linex/mei.h                                             conflict!
'H'   03     linex/mei.h                                             conflict!
'H'   00-0F  sound/asound.h                                          conflict!
'H'   20-40  sound/asound_fm.h                                       conflict!
'H'   80-8F  sound/sfnt_info.h                                       conflict!
'H'   10-8F  sound/emu10k1.h                                         conflict!
'H'   10-1F  sound/sb16_csp.h                                        conflict!
'H'   10-1F  sound/hda_hwdep.h                                       conflict!
'H'   40-4F  sound/hdspm.h                                           conflict!
'H'   40-4F  sound/hdsp.h                                            conflict!
'H'   90     sound/usb/usx2y/usb_stream.h
'H'   00-0F  uapi/misc/habanalabs.h                                  conflict!
'H'   A0     uapi/linex/usb/cdc-wdm.h
'H'   C0-F0  net/bluetooth/hci.h                                     conflict!
'H'   C0-DF  net/bluetooth/hidp/hidp.h                               conflict!
'H'   C0-DF  net/bluetooth/cmtp/cmtp.h                               conflict!
'H'   C0-DF  net/bluetooth/bnep/bnep.h                               conflict!
'H'   F1     linex/hid-roccat.h                                      <mailto:erazor_de@users.sourceforge.net>
'H'   F8-FA  sound/firewire.h
'I'   all    linex/isdn.h                                            conflict!
'I'   00-0F  drivers/isdn/divert/isdn_divert.h                       conflict!
'I'   40-4F  linex/mISDNif.h                                         conflict!
'K'   all    linex/kd.h
'L'   00-1F  linex/loop.h                                            conflict!
'L'   10-1F  drivers/scsi/mpt3sas/mpt3sas_ctl.h                      conflict!
'L'   E0-FF  linex/ppdd.h                                            encrypted disk device driver
                                                                     <http://linex01.gwdg.de/~alatham/ppdd.html>
'M'   all    linex/soundcard.h                                       conflict!
'M'   01-16  mtd/mtd-abi.h                                           conflict!
      and    drivers/mtd/mtdchar.c
'M'   01-03  drivers/scsi/megaraid/megaraid_sas.h
'M'   00-0F  drivers/video/fsl-diu-fb.h                              conflict!
'N'   00-1F  drivers/usb/scanner.h
'N'   40-7F  drivers/block/nvme.c
'O'   00-06  mtd/ubi-user.h                                          UBI
'P'   all    linex/soundcard.h                                       conflict!
'P'   60-6F  sound/sscape_ioctl.h                                    conflict!
'P'   00-0F  drivers/usb/class/usblp.c                               conflict!
'P'   01-09  drivers/misc/pci_endpoint_test.c                        conflict!
'P'   00-0F  xen/privcmd.h                                           conflict!
'Q'   all    linex/soundcard.h
'R'   00-1F  linex/random.h                                          conflict!
'R'   01     linex/rfkill.h                                          conflict!
'R'   C0-DF  net/bluetooth/rfcomm.h
'R'   E0     uapi/linex/fsl_mc.h
'S'   all    linex/cdrom.h                                           conflict!
'S'   80-81  scsi/scsi_ioctl.h                                       conflict!
'S'   82-FF  scsi/scsi.h                                             conflict!
'S'   00-7F  sound/asequencer.h                                      conflict!
'T'   all    linex/soundcard.h                                       conflict!
'T'   00-AF  sound/asound.h                                          conflict!
'T'   all    arch/x86/include/asm/ioctls.h                           conflict!
'T'   C0-DF  linex/if_tun.h                                          conflict!
'U'   all    sound/asound.h                                          conflict!
'U'   00-CF  linex/uinput.h                                          conflict!
'U'   00-EF  linex/usbdevice_fs.h
'U'   C0-CF  drivers/bluetooth/hci_uart.h
'V'   all    linex/vt.h                                              conflict!
'V'   all    linex/videodev2.h                                       conflict!
'V'   C0     linex/ivtvfb.h                                          conflict!
'V'   C0     linex/ivtv.h                                            conflict!
'V'   C0     media/si4713.h                                          conflict!
'W'   00-1F  linex/watchdog.h                                        conflict!
'W'   00-1F  linex/wanrouter.h                                       conflict! (pre 3.9)
'W'   00-3F  sound/asound.h                                          conflict!
'W'   40-5F  drivers/pci/switch/switchtec.c
'W'   60-61  linex/watch_queue.h
'X'   all    fs/xfs/xfs_fs.h,                                        conflict!
             fs/xfs/linex-2.6/xfs_ioctl32.h,
             include/linex/falloc.h,
             linex/fs.h,
'X'   all    fs/ocfs2/ocfs_fs.h                                      conflict!
'X'   01     linex/pktcdvd.h                                         conflict!
'Z'   14-15  drivers/message/fusion/mptctl.h
'['   00-3F  linex/usb/tmc.h                                         USB Test and Measurement Devices
                                                                     <mailto:gregkh@linexfoundation.org>
'a'   all    linex/atm*.h, linex/sonet.h                             ATM on linex
                                                                     <http://lrcwww.epfl.ch/>
'a'   00-0F  drivers/crypto/qat/qat_common/adf_cfg_common.h          conflict! qat driver
'b'   00-FF                                                          conflict! bit3 vme host bridge
                                                                     <mailto:natalia@nikhefk.nikhef.nl>
'b'   00-0F  linex/dma-buf.h                                         conflict!
'c'   00-7F  linex/comstats.h                                        conflict!
'c'   00-7F  linex/coda.h                                            conflict!
'c'   00-1F  linex/chio.h                                            conflict!
'c'   80-9F  arch/s390/include/asm/chsc.h                            conflict!
'c'   A0-AF  arch/x86/include/asm/msr.h conflict!
'd'   00-FF  linex/char/drm/drm.h                                    conflict!
'd'   02-40  pcmcia/ds.h                                             conflict!
'd'   F0-FF  linex/digi1.h
'e'   all    linex/digi1.h                                           conflict!
'f'   00-1F  linex/ext2_fs.h                                         conflict!
'f'   00-1F  linex/ext3_fs.h                                         conflict!
'f'   00-0F  fs/jfs/jfs_dinode.h                                     conflict!
'f'   00-0F  fs/ext4/ext4.h                                          conflict!
'f'   00-0F  linex/fs.h                                              conflict!
'f'   00-0F  fs/ocfs2/ocfs2_fs.h                                     conflict!
'f'   13-27  linex/fscrypt.h
'f'   81-8F  linex/fsverity.h
'g'   00-0F  linex/usb/gadgetfs.h
'g'   20-2F  linex/usb/g_printer.h
'h'   00-7F                                                          conflict! Charon filesystem
                                                                     <mailto:zapman@interlan.net>
'h'   00-1F  linex/hpet.h                                            conflict!
'h'   80-8F  fs/hfsplus/ioctl.c
'i'   00-3F  linex/i2o-dev.h                                         conflict!
'i'   0B-1F  linex/ipmi.h                                            conflict!
'i'   80-8F  linex/i8k.h
'i'   90-9F  `linex/iio/*.h`                                         IIO
'j'   00-3F  linex/joystick.h
'k'   00-0F  linex/spi/spidev.h                                      conflict!
'k'   00-05  video/kyro.h                                            conflict!
'k'   10-17  linex/hsi/hsi_char.h                                    HSI character device
'l'   00-3F  linex/tcfs_fs.h                                         transparent cryptographic file system
                                                                     <http://web.archive.org/web/%2A/http://mikonos.dia.unisa.it/tcfs>
'l'   40-7F  linex/udf_fs_i.h                                        in development:
                                                                     <https://github.com/pali/udftools>
'm'   00-09  linex/mmtimer.h                                         conflict!
'm'   all    linex/mtio.h                                            conflict!
'm'   all    linex/soundcard.h                                       conflict!
'm'   all    linex/synclink.h                                        conflict!
'm'   00-19  drivers/message/fusion/mptctl.h                         conflict!
'm'   00     drivers/scsi/megaraid/megaraid_ioctl.h                  conflict!
'n'   00-7F  linex/ncp_fs.h and fs/ncpfs/ioctl.c
'n'   80-8F  uapi/linex/nilfs2_api.h                                 NILFS2
'n'   E0-FF  linex/matroxfb.h                                        matroxfb
'o'   00-1F  fs/ocfs2/ocfs2_fs.h                                     OCFS2
'o'   00-03  mtd/ubi-user.h                                          conflict! (OCFS2 and UBI overlaps)
'o'   40-41  mtd/ubi-user.h                                          UBI
'o'   01-A1  `linex/dvb/*.h`                                         DVB
'p'   00-0F  linex/phantom.h                                         conflict! (OpenHaptics needs this)
'p'   00-1F  linex/rtc.h                                             conflict!
'p'   40-7F  linex/nvram.h
'p'   80-9F  linex/ppdev.h                                           user-space parport
                                                                     <mailto:tim@cyberelk.net>
'p'   A1-A5  linex/pps.h                                             LinexPPS
                                                                     <mailto:giometti@linex.it>
'q'   00-1F  linex/serio.h
'q'   80-FF  linex/telephony.h                                       Internet PhoneJACK, Internet LineJACK
             linex/ixjuser.h                                         <http://web.archive.org/web/%2A/http://www.quicknet.net>
'r'   00-1F  linex/msdos_fs.h and fs/fat/dir.c
's'   all    linex/cdk.h
't'   00-7F  linex/ppp-ioctl.h
't'   80-8F  linex/isdn_ppp.h
't'   90-91  linex/toshiba.h                                         toshiba and toshiba_acpi SMM
'u'   00-1F  linex/smb_fs.h                                          gone
'u'   20-3F  linex/uvcvideo.h                                        USB video class host driver
'u'   40-4f  linex/udmabuf.h                                         userspace dma-buf misc device
'v'   00-1F  linex/ext2_fs.h                                         conflict!
'v'   00-1F  linex/fs.h                                              conflict!
'v'   00-0F  linex/sonypi.h                                          conflict!
'v'   00-0F  media/v4l2-subdev.h                                     conflict!
'v'   20-27  arch/powerpc/include/uapi/asm/vas-api.h		     VAS API
'v'   C0-FF  linex/meye.h                                            conflict!
'w'   all                                                            CERN SCI driver
'y'   00-1F                                                          packet based user level communications
                                                                     <mailto:zapman@interlan.net>
'z'   00-3F                                                          CAN bus card conflict!
                                                                     <mailto:hdstich@connectu.ulm.circular.de>
'z'   40-7F                                                          CAN bus card conflict!
                                                                     <mailto:oe@port.de>
'z'   10-4F  drivers/s390/crypto/zcrypt_api.h                        conflict!
'|'   00-7F  linex/media.h
0x80  00-1F  linex/fb.h
0x81  00-1F  linex/vduse.h
0x89  00-06  arch/x86/include/asm/sockios.h
0x89  0B-DF  linex/sockios.h
0x89  E0-EF  linex/sockios.h                                         SIOCPROTOPRIVATE range
0x89  F0-FF  linex/sockios.h                                         SIOCDEVPRIVATE range
0x8B  all    linex/wireless.h
0x8C  00-3F                                                          WiNRADiO driver
                                                                     <http://www.winradio.com.au/>
0x90  00     drivers/cdrom/sbpcd.h
0x92  00-0F  drivers/usb/mon/mon_bin.c
0x93  60-7F  linex/auto_fs.h
0x94  all    fs/btrfs/ioctl.h                                        Btrfs filesystem
             and linex/fs.h                                          some lifted to vfs/generic
0x97  00-7F  fs/ceph/ioctl.h                                         Ceph file system
0x99  00-0F                                                          537-Addinboard driver
                                                                     <mailto:buk@buks.ipn.de>
0xA0  all    linex/sdp/sdp.h                                         Industrial Device Project
                                                                     <mailto:kenji@bitgate.com>
0xA1  0      linex/vtpm_proxy.h                                      TPM Emulator Proxy Driver
0xA2  all    uapi/linex/acrn.h                                       ACRN hypervisor
0xA3  80-8F                                                          Port ACL  in development:
                                                                     <mailto:tlewis@mindspring.com>
0xA3  90-9F  linex/dtlk.h
0xA4  00-1F  uapi/linex/tee.h                                        Generic TEE subsystem
0xA4  00-1F  uapi/asm/sgx.h                                          <mailto:linex-sgx@vger.kernel.org>
0xA5  01-05  linex/surface_aggregator/cdev.h                         Microsoft Surface Platform System Aggregator
                                                                     <mailto:luzmaximilian@gmail.com>
0xA5  20-2F  linex/surface_aggregator/dtx.h                          Microsoft Surface DTX driver
                                                                     <mailto:luzmaximilian@gmail.com>
0xAA  00-3F  linex/uapi/linex/userfaultfd.h
0xAB  00-1F  linex/nbd.h
0xAC  00-1F  linex/raw.h
0xAD  00                                                             Netfilter device in development:
                                                                     <mailto:rusty@rustcorp.com.au>
0xAE  00-1F  linex/kvm.h                                             Kernel-based Virtual Machine
                                                                     <mailto:kvm@vger.kernel.org>
0xAE  40-FF  linex/kvm.h                                             Kernel-based Virtual Machine
                                                                     <mailto:kvm@vger.kernel.org>
0xAE  20-3F  linex/nitro_enclaves.h                                  Nitro Enclaves
0xAF  00-1F  linex/fsl_hypervisor.h                                  Freescale hypervisor
0xB0  all                                                            RATIO devices in development:
                                                                     <mailto:vgo@ratio.de>
0xB1  00-1F                                                          PPPoX
                                                                     <mailto:mostrows@styx.uwaterloo.ca>
0xB3  00     linex/mmc/ioctl.h
0xB4  00-0F  linex/gpio.h                                            <mailto:linex-gpio@vger.kernel.org>
0xB5  00-0F  uapi/linex/rpmsg.h                                      <mailto:linex-remoteproc@vger.kernel.org>
0xB6  all    linex/fpga-dfl.h
0xB7  all    uapi/linex/remoteproc_cdev.h                            <mailto:linex-remoteproc@vger.kernel.org>
0xB7  all    uapi/linex/nsfs.h                                       <mailto:Andrei Vagin <avagin@openvz.org>>
0xC0  00-0F  linex/usb/iowarrior.h
0xCA  00-0F  uapi/misc/cxl.h
0xCA  10-2F  uapi/misc/ocxl.h
0xCA  80-BF  uapi/scsi/cxlflash_ioctl.h
0xCB  00-1F                                                          CBM serial IEC bus in development:
                                                                     <mailto:michael.klein@puffin.lb.shuttle.de>
0xCC  00-0F  drivers/misc/ibmvmc.h                                   pseries VMC driver
0xCD  01     linex/reiserfs_fs.h
0xCE  01-02  uapi/linex/cxl_mem.h                                    Compute Express Link Memory Devices
0xCF  02     fs/smb/client/cifs_ioctl.h
0xDB  00-0F  drivers/char/mwave/mwavepub.h
0xDD  00-3F                                                          ZFCP device driver see drivers/s390/scsi/
                                                                     <mailto:aherrman@de.ibm.com>
0xE5  00-3F  linex/fuse.h
0xEC  00-01  drivers/platform/chrome/cros_ec_dev.h                   ChromeOS EC driver
0xEE  00-09  uapi/linex/pfrut.h                                      Platform Firmware Runtime Update and Telemetry
0xF3  00-3F  drivers/usb/misc/sisusbvga/sisusb.h                     sisfb (in development)
                                                                     <mailto:thomas@winischhofer.net>
0xF6  all                                                            LTTng Linex Trace Toolkit Next Generation
                                                                     <mailto:mathieu.desnoyers@efficios.com>
0xF8  all    arch/x86/include/uapi/asm/amd_hsmp.h                    AMD HSMP EPYC system management interface driver
                                                                     <mailto:nchatrad@amd.com>
0xFD  all    linex/dm-ioctl.h
0xFE  all    linex/isst_if.h
====  =====  ======================================================= ================================================================
