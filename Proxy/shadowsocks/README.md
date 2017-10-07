# Shadowsocks

`Shadowsocks`基于`Python`开发，配置`Shadowsocks`时需要同时部署服务端和客户端。

由`Google`开源的`BBR`拥塞控制算法可以安装在`Shadowsocks`服务端，加速你的代理。但是`BBR`只支持`KVM`虚拟化技术，不支持`OpenVZ`，如果你有一个`OpenVZ`的虚拟主机，可以采用搭建`UML`的方式创建一个子虚拟机，使得其支持`BBR`技术。

`OpenVZ`上部署`BBR`，见[OpenVZ-BBR](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/shadowsocks/OpenVZ-BBR.md)。

部署服务端`Shadowsocks`，见[Shadowsocks-Server](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/shadowsocks/Shadowsocks-Server.md)。

部署客户端`Shadowsocks`，见[Shadowsocks-Client](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/shadowsocks/Shadowsocks-Client.md)。

`Ubuntu`环境下本地应用代理的配置，见[Ubuntu-App-config](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/Linux-App-config.md)。

