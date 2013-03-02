#!/bin/bash
ip link set wlan0 up
wpa_supplicant -B -Dwext -iwlan0 -c/etc/wpa_supplicant.conf 
dhcpcd wlan0

