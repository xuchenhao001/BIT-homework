# Proxy

这里是我关于几种代理方式的研究与实践结果，希望能够帮助那些需要越过`GFW`的同胞们。目前包括以下方法：

* Shadowsocks
* Squid + Stunnel
* SSH proxy

其中，我认为`Shadowsocks`代理作为平时使用最佳，因为可以采取`PAC`策略，控制某些网站自动走代理。而`Squid`+`Stunnel`方式次之，连接也较为稳定。最后，当环境实在不允许时，也可采用走`SSH`通道的代理方案，此种方法经测试稳定性较差，不建议长期使用。

## Shadowsocks

基于`Python`开发，需要服务端安装相应的服务程序监听，客户端需要装相应的客户端进行代理。

优点：可以采取`PAC`策略，控制某些网站自动走代理。具有身份认证功能。支持多种加密方式，客户端支持`Windows`、`Linux`、`Android`和`iOS`。

缺点：服务端与客户端通信利用了`Socks5`而非`HTTP`协议，所以常用的一些基于`HTTP`协议的应用程序需要将其消息封装为`Socks5`协议后发送，有些资源浪费。

安装配置参见[此文](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/shadowsocks/README.md)。

## Squid + Stunnel

`Squid`用来反向代理客户端发送的消息，`Stunnel`用`TLS/SSL`协议来加密客户端和服务端的通信，从而绕过`GFW`的监测。

优点：利用`HTTPS`协议进行数据传输，对多数应用程序的支持较好。客户端支持`Windows`、`Linux`，并且开源。

缺点：不支持`PAC`模式，必须同时代理所有网址。且我目前没有搞定身份认证问题，不具有身份认证功能。

安装配置参见[此文](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/squid-stunnel/README.md)。

## SSH proxy

这是我所见过的兼容性最强的代理方式。客户端和服务端通过`SSH`通道进行代理。服务端不需要安装任何程序，只要打开了`22`端口，能够`SSH`连接即可。客户端也不需要装多余程序，只需要`putty`，能够远程连接到服务端即可。

优点：利用`SSH`通道进行数据传输，有身份认证，安全性强，且对服务端的兼容性极强。

缺点：不支持`PAC`，仅支持`Windows`客户端，且要安装`putty`。容易断线，建议仅作为浏览器的临时代理使用。

安装配置参见[此文](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/SSH-proxy/README.md)。

