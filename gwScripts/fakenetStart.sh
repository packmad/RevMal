#!/usr/bin/env bash

if [[ $(id -u) -ne 0 ]] ; then echo "> Run with sudo or as root!" ; exit 1 ; fi

systemctl stop systemd-resolved

ip link set ens33 down

fakenet
