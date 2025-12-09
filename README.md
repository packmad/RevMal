# RevMal
RevMal Course Material




## VM


### Enable natting

Add `net.ipv4.ip_forward=1` to `/etc/sysctl.conf`

`/etc/nftables.conf`
```
#!/usr/sbin/nft -f

flush ruleset

# ---- Adjust these if names/subnet change ----
define LAN_IF  = "ens37"
define WAN_IF  = "ens33"
define LAN_NET = 192.168.255.0/24
# --------------------------------------------

# NAT: masquerade LAN traffic out the WAN
table ip nat {
  chain postrouting {
    type nat hook postrouting priority srcnat;
    oifname $WAN_IF ip saddr $LAN_NET masquerade
  }
}

# Forwarding policy (stateful): allow LAN->WAN and related return traffic
table inet filter {
  chain forward {
    type filter hook forward priority filter;

    # allow established/related packets (return traffic)
    ct state established,related accept

    # allow new connections from LAN to WAN
    iifname $LAN_IF oifname $WAN_IF ip saddr $LAN_NET accept

    # (optional) allow inter-LAN forwarding if you later add more LANs
    # iifname $LAN_IF oifname $LAN_IF accept

    # NEW: allow LAN -> Docker bridge(s) for published KMS port
    iifname $LAN_IF oifname "docker0" ip saddr $LAN_NET tcp dport 1688 accept
    iifname $LAN_IF oifname "br-*"  ip saddr $LAN_NET tcp dport 1688 accept

    # everything else gets dropped (and counted)
    counter drop
  }
}
```

Then `systemctl enable --now nftables`

[FakeNet-NG](https://github.com/mandiant/flare-fakenet-ng)

`pip install -U FakeNet-NG --break-system-packages`