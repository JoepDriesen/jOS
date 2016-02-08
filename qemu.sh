#!/bin/sh
set -e
. ./clean.sh
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom jOS.iso -gdb tcp::1234
