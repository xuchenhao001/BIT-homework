# OpenVZ Ubuntu下部署UML，启用BBR，加速shadowsocks

OpengVZ，基于Linux内核和作業系統的操作系统级虚拟化技术。部署UML（User-mode Linux）后，虚拟一个具有BBR加速的小型操作系统。之后在小型操作系统中运行shadowsocks server，在host中将所有请求都转发到小型操作系统，以使用BBR，达到加速shadowsocks的目的。
摘自91yun.org，亲测可用。硬件需求：

* 主机支持TUN/TAP
* CPU 1 核
* RAM 256M+
* Disk 5GB

## 前置安装：

```
$ apt-get install e2fsprogs uml-utilities screen
```

## OpenVZ主机上的网络设置

先设置一个tap虚拟网络设备`tap0`，设置子虚拟机的网络为`10.0.0.1`，启用`tap0`设备，开启`iptable`转发：
```
$ ip tuntap add tap0 mode tap
$ ip addr add 10.0.0.1/24 dev tap0
$ ip link set tap0 up
$ iptables -P FORWARD ACCEPT
$ iptables -t nat -A POSTROUTING -o venet0 -j MASQUERADE
```

## OpenVZ主机上转发规则设置

预留`22`端口给ssh：
```
$ iptables -t nat -A PREROUTING -p tcp --dport 22 -j RETURN
```
全部消息转发子虚拟机（子虚拟机的网络地址为`10.0.0.2`）：
```
$ iptables -t nat -A PREROUTING -i venet0 -j DNAT --to-destination 10.0.0.2
```

如果不想全部消息都转发子虚拟机，可以这样只将`10000`~`20000`端口的消息：
```
$ iptables -t nat -A PREROUTING -i venet0 -p tcp --dport 10000:20000 -j DNAT --to-destination 10.0.0.2
$ iptables -t nat -A PREROUTING -i venet0 -p udp --dport 10000:20000 -j DNAT --to-destination 10.0.0.2
```

之后下载91yun.org为我们提供的uml内核（后续我可能会自己编译一个uml，如果有机会的话）：

```
#32位系统下载该压缩包
$ wget http://soft.91yun.org/uml/32/uml.tar.xz 
#64位系统下载该压缩包
$ wget http://soft.91yun.org/uml/64/uml.tar.xz 
$ tar xvJf uml.tar.xz
```

进入解压缩后的文件夹，发现里面有两个文件：`vmlinux` (kernel)和`rootfs` (filesystem image)。整个子虚拟机都在`rootfs`内，子虚拟机的网络已经被设置好了。

`rootfs`一开始只有`300M`，为了方便我们在里面安装程序，我们需要先扩大image的容量，这里我扩容到`5GB`：

```
$ resize2fs rootfs 5G
```

## 打开user-mode linux

前台运行子虚拟机：（一直不能关闭开启子虚拟机的ssh）

```
$ ./vmlinux ubda=rootfs eth0=tuntap,tap0 mem=256m
```

> 要想在后台运行子虚拟机：
> `$ nohup ./vmlinux ubda=rootfs eth0=tuntap,tap0 mem=256m &disown`

其中，`mem=256m`表示分配给子虚拟机`256M`的RAM大小。

等待子虚拟机成功启动，出现如下内容：

```
Virtual console 3 assigned device '/dev/pts/1'
Virtual console 4 assigned device '/dev/pts/2'
Virtual console 5 assigned device '/dev/pts/3'
Virtual console 6 assigned device '/dev/pts/4'
Virtual console 2 assigned device '/dev/pts/5'
Virtual console 1 assigned device '/dev/pts/6'
```

表明启动完成，其中`/dev/pts/x`就是子虚拟机放`virtual console`的地方，随便记住一个，另开一个`terminal`远程到主机上，用以下命令进入子虚拟机：

```
$ screen /dev/pts/1
```

进去后屏幕应该是什么都没有，按一下`Enter`就会出现登陆提示。

> 登陆账户为：root
> 登陆密码为：91yun.org

进入系统后，可以运行一下如下指令：

```
$ sysctl net.ipv4.tcp_available_congestion_control
```

如果返回内容如下：

```
net.ipv4.tcp_available_congestion_control = bbr reno cubic highspeed
```

表明BBR已经在运行了。

## 部署shadowsocks server

首先安装`python`和`pip`：

```
$ apt install python python-pip
```

安装`shadowsocks`：

```
$ pip install git+https://github.com/shadowsocks/shadowsocks.git@master
```

新建`/etc/shadowsocks.json`配置文件：

```
$ vi /etc/shadowsocks.json
```

填入配置内容，例如：

```
{
    "server":"my_server_ip",
    "server_port":10221,
    "local_address": "127.0.0.1",
    "local_port":1080,
    "password":"mypassword",
    "timeout":300,
    "method":"aes-256-cfb",
    "fast_open": false
}
```

后台启动`shadowsocks`：

```
$ ssserver -c /etc/shadowsocks.json -d start
```

此时已经在子虚拟机中成功启动了`shadowsocks`，并且发送到OpenVZ主机的消息都会转发到子虚拟机中。

## 本地Ubuntu Server连接`shadowsocks`代理

安装与配置`shadowsocks`步骤与上面服务器端配置相同，要注意将配置文件中`server`的值设置成你的OpenVZ主机地址。服务器端口要统一，加密方式一定要统一。

本地后台启动`shadowsocks`客户端：

```
$ sslocal -c /etc/shadowsocks.json -d start
```

本地安装`privoxy`将http消息代理为`socks5`：

```
$ apt install privoxy
$ vim /etc/privoxy/config
```

去掉这一行的注释：

```
forward-socks5t   /               127.0.0.1:1080 .
```

开启`privoxy`代理：

```
$ service privoxy start 
```

现在，如果你想在`terminal`里走`socks5`代理：（其中`8118`就是`privoxy`的默认代理端口，本地`shadowsocks`的代理端口是`1080`）

```
$ export http_proxy='http://localhost:8118'
$ export https_proxy='https://localhost:8118'
```

可以查看一下是否成功连接了`shadowsocks server`：

```
$ curl ip.gs
```

如果返回的不是你当前机器的IP地址，而是国外的IP，恭喜你，代理成功。