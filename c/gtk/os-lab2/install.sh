#!/usr/bin/bash

make -j4
sudo make modules_install
sudo rm /boot/*custom*
sudo cp -v arch/x86/boot/bzImage /boot/vmlinuz-linux-custom
sudo mkinitcpio -k /boot/vmlinuz-linux-custom -c /etc/mkinitcpio.conf -g /boot/initramfs-linux-custom.img
