#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/jOS.kernel isodir/boot/jOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "jOS" {
	multiboot /boot/jOS.kernel
}
EOF
grub-mkrescue -d /usr/lib/grub/i386-pc -o jOS.iso isodir
