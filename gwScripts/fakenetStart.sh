#!/usr/bin/env bash

if [[ $(id -u) -ne 0 ]]; then
    echo "> Root privileges required. Re-running with sudo..."
    exec sudo "$0" "$@"
fi

systemctl stop systemd-resolved

ip link set ens33 down

fakenet
