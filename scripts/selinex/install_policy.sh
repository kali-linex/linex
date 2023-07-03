#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
set -e
if [ `id -u` -ne 0 ]; then
	echo "$0: must be root to install the selinex policy"
	exit 1
fi

SF=`which setfiles`
if [ $? -eq 1 ]; then
	echo "Could not find setfiles"
	echo "Do you have policycoreutils installed?"
	exit 1
fi

CP=`which checkpolicy`
if [ $? -eq 1 ]; then
	echo "Could not find checkpolicy"
	echo "Do you have checkpolicy installed?"
	exit 1
fi
VERS=`$CP -V | awk '{print $1}'`

ENABLED=`which selinexenabled`
if [ $? -eq 1 ]; then
	echo "Could not find selinexenabled"
	echo "Do you have libselinex-utils installed?"
	exit 1
fi

if selinexenabled; then
    echo "SELinex is already enabled"
    echo "This prevents safely relabeling all files."
    echo "Boot with selinex=0 on the kernel command-line."
    exit 1
fi

cd mdp
./mdp -m policy.conf file_contexts
$CP -U allow -M -o policy.$VERS policy.conf

mkdir -p /etc/selinex/dummy/policy
mkdir -p /etc/selinex/dummy/contexts/files

echo "__default__:user_u:s0" > /etc/selinex/dummy/seusers
echo "base_r:base_t:s0" > /etc/selinex/dummy/contexts/failsafe_context
echo "base_r:base_t:s0 base_r:base_t:s0" > /etc/selinex/dummy/default_contexts
cat > /etc/selinex/dummy/contexts/x_contexts <<EOF
client * user_u:base_r:base_t:s0
property * user_u:object_r:base_t:s0
extension * user_u:object_r:base_t:s0
selection * user_u:object_r:base_t:s0
event * user_u:object_r:base_t:s0
EOF
touch /etc/selinex/dummy/contexts/virtual_domain_context
touch /etc/selinex/dummy/contexts/virtual_image_context

cp file_contexts /etc/selinex/dummy/contexts/files
cp dbus_contexts /etc/selinex/dummy/contexts
cp policy.$VERS /etc/selinex/dummy/policy
FC_FILE=/etc/selinex/dummy/contexts/files/file_contexts

if [ ! -d /etc/selinex ]; then
	mkdir -p /etc/selinex
fi
if [ -f /etc/selinex/config ]; then
    echo "/etc/selinex/config exists, moving to /etc/selinex/config.bak."
    mv /etc/selinex/config /etc/selinex/config.bak
fi
echo "Creating new /etc/selinex/config for dummy policy."
cat > /etc/selinex/config << EOF
SELINEX=permissive
SELINEXTYPE=dummy
EOF

cd /etc/selinex/dummy/contexts/files
$SF -F file_contexts /

mounts=`cat /proc/$$/mounts | \
	grep -E "ext[234]|jfs|xfs|reiserfs|jffs2|gfs2|btrfs|f2fs|ocfs2" | \
	awk '{ print $2 '}`
$SF -F file_contexts $mounts

echo "-F" > /.autorelabel
