# Deploy Cisco AnyConnect VPN

This guide besed on `Cisco AnnyConnect`, the best vpn solution for enterprise.

## Server：

Install `ocserv`:

```bash
$ apt install ocserv
```

Generate server certificate:

```bash
$ cd /etc/ocserv
$ mkdir certs
$ openssl req -nodes -newkey rsa:2048 -keyout certs/server.key -out certs/server.csr -subj "/C=/ST=/L=/O=/OU=/CN=mycn"
$ openssl x509 -req -sha256 -days 3650 -in certs/server.csr -signkey certs/server.key -out certs/server.crt
```

Configure the OpenConnect VPN server:

```bash
$ cd /etc/ocserv
$ vim ocserv.conf
# Use editor modify these parameters

#auth = "pam[gid-min=1000]"
auth = "plain[/etc/ocserv/ocpasswd]"

#server-cert = /etc/ssl/certs/ssl-cert-snakeoil.pem
#server-key = /etc/ssl/private/ssl-cert-snakeoil.key
server-cert = /etc/ocserv/certs/server.crt
server-key = /etc/ocserv/certs/server.key

#try-mtu-discovery = false
try-mtu-discovery = true

#dns = 192.168.1.2
dns = 8.8.8.8

# Comment out all route fields
#route = 10.10.10.0/255.255.255.0
#route = 192.168.0.0/255.255.0.0
#route = fef4:db8:1000:1001::/64

#no-route = 192.168.5.0/255.255.255.0

cisco-client-compat = true
```

Create user id and password:

```bash
$ ocpasswd -c /etc/ocserv/ocpasswd <your-id>
```

> Then you need to type in your password twice.

Enable packet forwarding:

```bash
$ vim /etc/sysctl.conf
# Uncomment this line
net.ipv4.ip_forward=1
# Then active forwarding
$ sysctl -p
```

Open the server firewall for SSL:

```bash
$ iptables -A INPUT -p tcp --dport 443 -j ACCEPT
$ iptables -A INPUT -p udp --dport 443 -j ACCEPT
```

Enable masquerade for snat:

```bash
$ iptables -t nat -A POSTROUTING -j MASQUERADE
```

Restart `ocserv`:

```bash
$ systemctl restart ocserv
```

## Client - ubuntu

1. First run command below to active the `TUN` module:

   ```bash
   $ /sbin/modprobe tun
   ```

2. Install OpenConnect:

   ```bash
   $ apt install openconnect
   ```

3. Connect to VPN, run:

   ```bash
   $ openconnect <your-vpn-ip-or-domain>
   ```

   ​