#!/usr/bin/bash
if [[ -n $1 ]]; then
    sudo mknod /dev/mydev c $1 0
    sudo chmod 666 /dev/mydev
else
    make
    sudo insmod mydev.ko
    grep mydev /proc/devices
fi
