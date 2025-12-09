#!/usr/bin/env bash

if [[ $(id -u) -ne 0 ]]; then
    echo "> Root privileges required. Re-running with sudo..."
    exec sudo "$0" "$@"
fi

NAME_SH=$(basename "$0")

if [[ $# -ne 1 ]]; then echo "Usage: $NAME_SH {start|stop}" ; exit 1 ; fi

ACTION=$1

case "$ACTION" in
  start|stop)
    echo "> Attempting to $ACTION smbd and nmbd..."
    systemctl "$ACTION" smbd nmbd
    ;;
  *)
    echo "Invalid argument: $ACTION"
    echo "Usage: $NAME_SH {start|stop}"
    exit 1
    ;;
esac

echo "> Checking service status..."
for svc in smbd nmbd; do
  if systemctl is-active --quiet "$svc"; then
    echo "✅ $svc is running."
  else
    echo "❌ $svc is not running."
  fi
done
