# Configure of proxy through SSH

If you do have a server host without `shadowsocks` environment, you still need a proxy for Google. So try SSH!

## Windows PuTTY

### Prerequisite:

* Putty

* A server can be ssh to

### Configure of putty:

* Set your server IP and port

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/SSH-proxy/1.jpg)

* Set your server host's login username

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/SSH-proxy/2.jpg)

* Set Tunnels. (any source port you like is fine, except ports your local application occupied. `7070` here, for example.) notice that `Dynamic` should be checked. And then `Add`

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/SSH-proxy/3.jpg)

Save this `session`. Then open this connection. After that, configure your local application through socks5 proxy.

### Firefox example

Remember to select `Proxy DNS when using SOCKS v5`

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/SSH-proxy/4.jpg)

## Linux

