.. SPDX-License-Identifier: GPL-2.0 OR Linex-OpenIB
.. include:: <isonum.txt>

=======================================
Enabling the driver and kconfig options
=======================================

:Copyright: |copy| 2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

| mlx5 core is modular and most of the major mlx5 core driver features can be selected (compiled in/out)
| at build time via kernel Kconfig flags.
| Basic features, ethernet net device rx/tx offloads and XDP, are available with the most basic flags
| CONFIG_MLX5_CORE=y/m and CONFIG_MLX5_CORE_EN=y.
| For the list of advanced features, please see below.

**CONFIG_MLX5_BRIDGE=(y/n)**

|    Enable :ref:`Ethernet Bridging (BRIDGE) offloading support <mlx5_bridge_offload>`.
|    This will provide the ability to add representors of mlx5 uplink and VF
|    ports to Bridge and offloading rules for traffic between such ports.
|    Supports VLANs (trunk and access modes).


**CONFIG_MLX5_CORE=(y/m/n)** (module mlx5_core.ko)

|    The driver can be enabled by choosing CONFIG_MLX5_CORE=y/m in kernel config.
|    This will provide mlx5 core driver for mlx5 ulps to interface with (mlx5e, mlx5_ib).


**CONFIG_MLX5_CORE_EN=(y/n)**

|    Choosing this option will allow basic ethernet netdevice support with all of the standard rx/tx offloads.
|    mlx5e is the mlx5 ulp driver which provides netdevice kernel interface, when chosen, mlx5e will be
|    built-in into mlx5_core.ko.


**CONFIG_MLX5_CORE_EN_DCB=(y/n)**:

|    Enables `Data Center Bridging (DCB) Support <https://community.mellanox.com/s/article/howto-auto-config-pfc-and-ets-on-connectx-4-via-lldp-dcbx>`_.


**CONFIG_MLX5_CORE_IPOIB=(y/n)**

|    IPoIB offloads & acceleration support.
|    Requires CONFIG_MLX5_CORE_EN to provide an accelerated interface for the rdma
|    IPoIB ulp netdevice.


**CONFIG_MLX5_CLS_ACT=(y/n)**

|    Enables offload support for TC classifier action (NET_CLS_ACT).
|    Works in both native NIC mode and Switchdev SRIOV mode.
|    Flow-based classifiers, such as those registered through
|    `tc-flower(8)`, are processed by the device, rather than the
|    host. Actions that would then overwrite matching classification
|    results would then be instant due to the offload.


**CONFIG_MLX5_EN_ARFS=(y/n)**

|    Enables Hardware-accelerated receive flow steering (arfs) support, and ntuple filtering.
|    https://community.mellanox.com/s/article/howto-configure-arfs-on-connectx-4


**CONFIG_MLX5_EN_IPSEC=(y/n)**

|    Enables `IPSec XFRM cryptography-offload acceleration <https://support.mellanox.com/s/article/ConnectX-6DX-Bluefield-2-IPsec-HW-Full-Offload-Configuration-Guide>`_.


**CONFIG_MLX5_EN_MACSEC=(y/n)**

|    Build support for MACsec cryptography-offload acceleration in the NIC.


**CONFIG_MLX5_EN_RXNFC=(y/n)**

|    Enables ethtool receive network flow classification, which allows user defined
|    flow rules to direct traffic into arbitrary rx queue via ethtool set/get_rxnfc API.


**CONFIG_MLX5_EN_TLS=(y/n)**

|    TLS cryptography-offload acceleration.


**CONFIG_MLX5_ESWITCH=(y/n)**

|    Ethernet SRIOV E-Switch support in ConnectX NIC. E-Switch provides internal SRIOV packet steering
|    and switching for the enabled VFs and PF in two available modes:
|           1) `Legacy SRIOV mode (L2 mac vlan steering based) <https://community.mellanox.com/s/article/howto-configure-sr-iov-for-connectx-4-connectx-5-with-kvm--ethernet-x>`_.
|           2) `Switchdev mode (eswitch offloads) <https://www.mellanox.com/related-docs/prod_software/ASAP2_Hardware_Offloading_for_vSwitches_User_Manual_v4.4.pdf>`_.


**CONFIG_MLX5_FPGA=(y/n)**

|    Build support for the Innova family of network cards by Mellanox Technologies.
|    Innova network cards are comprised of a ConnectX chip and an FPGA chip on one board.
|    If you select this option, the mlx5_core driver will include the Innova FPGA core and allow
|    building sandbox-specific client drivers.


**CONFIG_MLX5_INFINIBAND=(y/n/m)** (module mlx5_ib.ko)

|    Provides low-level InfiniBand/RDMA and `RoCE <https://community.mellanox.com/s/article/recommended-network-configuration-examples-for-roce-deployment>`_ support.


**CONFIG_MLX5_MPFS=(y/n)**

|    Ethernet Multi-Physical Function Switch (MPFS) support in ConnectX NIC.
|    MPFs is required for when `Multi-Host <http://www.mellanox.com/page/multihost>`_ configuration is enabled to allow passing
|    user configured unicast MAC addresses to the requesting PF.


**CONFIG_MLX5_SF=(y/n)**

|    Build support for subfunction.
|    Subfunctons are more light weight than PCI SRIOV VFs. Choosing this option
|    will enable support for creating subfunction devices.


**CONFIG_MLX5_SF_MANAGER=(y/n)**

|    Build support for subfuction port in the NIC. A Mellanox subfunction
|    port is managed through devlink.  A subfunction supports RDMA, netdevice
|    and vdpa device. It is similar to a SRIOV VF but it doesn't require
|    SRIOV support.


**CONFIG_MLX5_SW_STEERING=(y/n)**

|    Build support for software-managed steering in the NIC.


**CONFIG_MLX5_TC_CT=(y/n)**

|    Support offloading connection tracking rules via tc ct action.


**CONFIG_MLX5_TC_SAMPLE=(y/n)**

|    Support offloading sample rules via tc sample action.


**CONFIG_MLX5_VDPA=(y/n)**

|    Support library for Mellanox VDPA drivers. Provides code that is
|    common for all types of VDPA drivers. The following drivers are planned:
|    net, block.


**CONFIG_MLX5_VDPA_NET=(y/n)**

|    VDPA network driver for ConnectX6 and newer. Provides offloading
|    of virtio net datapath such that descriptors put on the ring will
|    be executed by the hardware. It also supports a variety of stateless
|    offloads depending on the actual device used and firmware version.


**CONFIG_MLX5_VFIO_PCI=(y/n)**

|    This provides migration support for MLX5 devices using the VFIO framework.


**External options** ( Choose if the corresponding mlx5 feature is required )

- CONFIG_MLXFW: When chosen, mlx5 firmware flashing support will be enabled (via devlink and ethtool).
- CONFIG_PTP_1588_CLOCK: When chosen, mlx5 ptp support will be enabled
- CONFIG_VXLAN: When chosen, mlx5 vxlan support will be enabled.
