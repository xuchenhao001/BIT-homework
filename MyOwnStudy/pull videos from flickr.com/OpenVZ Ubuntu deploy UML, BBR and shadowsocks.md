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

先设置一个tap虚拟网络设备`tap0`，设置子虚拟机的网络为`10.0.0.1`，启用`tap0`设备，添加`venet0`转发规则到`iptable`转发表的`POSTROUTING`链上：

```
$ ip tuntap add tap0 mode tap
$ ip addr add 10.0.0.1/24 dev tap0
$ ip link set tap0 up
$ iptables -P FORWARD ACCEPT
$ iptables -t nat -A POSTROUTING -o venet0 -j MASQUERADE
```

检查`iptables`转发表的`POSTROUTING`链：

```
$ iptables -t nat -vnL POSTROUTING --line-number
```
可以看到如下规则被写入链中：

```
Chain POSTROUTING (policy ACCEPT 109 packets, 6460 bytes)
num   pkts bytes target     prot opt in     out     source               destination         
1      115  6900 MASQUERADE  all  --  *      venet0  0.0.0.0/0            0.0.0.0/0 
```

## OpenVZ主机上转发规则设置

预留`22`端口给`SSH`：
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

检查`iptables`转发表的`PREROUTING`链：

```
$ iptables -t nat -vnL PREROUTING --line-number
```

可以看到如下内容被写入`PREROUTING`链，表明当前`10000`~`20000`部分端口已经开启转发至虚拟机：

```
Chain PREROUTING (policy ACCEPT 430 packets, 25730 bytes)
num   pkts bytes target     prot opt in     out     source               destination         
1       25  1368 RETURN     tcp  --  *      *       0.0.0.0/0            0.0.0.0/0            tcp dpt:22
2      385 23020 DNAT       tcp  --  venet0 *       0.0.0.0/0            0.0.0.0/0            tcp dpts:10000:20000 to:10.0.0.2
3        0     0 DNAT       udp  --  venet0 *       0.0.0.0/0            0.0.0.0/0            udp dpts:10000:20000 to:10.0.0.2
```

删除某条规则，比如第二条，可以使用：

```
iptables -t nat -D PREROUTING 2
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

> 从screen中分离（从虚拟机中脱离）：`Ctrl-a`+`d`
>
> 重新连接你的screen：`screen -r`

## 部署shadowsocks server

首先更新一下数据源，升级一下软件包：

```
apt-get update
apt-get upgrade
```

安装`python`和`pip`：

```
$ apt install python python-pip
```

安装`shadowsocks`：

```
$ pip install shadowsocks
```
或最新版
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

## 本地Ubuntu Server连接`shadowsocks`代理服务器

有关本地`Shell`或者其他应用走`shadowsocks`代理的问题，请参考[此文](https://github.com/xuchenhao001/BIT-homework/blob/master/MyOwnStudy/pull%20videos%20from%20flickr.com/Local%20Ubuntu%20Server%20Connect%20to%20shadowsocks%20proxy.md)。