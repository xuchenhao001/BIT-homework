# OpenVZ Ubuntu下利用UML部署BBR

`OpenVZ`是一个操作系统级的虚拟化技术，本身并不支持`BBR`拥塞避免算法。但是部署`UML`（User-mode Linux）后，我们虚拟出一个具有`BBR`加速的子虚拟机。之后在子虚拟机中运行代理服务程序的服务端，再将原`OpenVZ`虚拟机中的所有代理请求都转发到`UML`子虚拟机中，以使用`BBR`算法加速代理。
摘自`91yun.org`，亲测可用。硬件需求：

* 主机支持`TUN/TAP`
* CPU 1 核
* RAM 256M+
* Disk 5GB

## 前置安装：

```bash
$ apt-get install e2fsprogs uml-utilities screen
```

## OpenVZ主机上的网络设置

先设置一个tap虚拟网络设备`tap0`，设置子虚拟机的网络为`10.0.0.1`，启用`tap0`设备，添加`venet0`转发规则到`iptable`转发表的`POSTROUTING`链上：

```bash
$ ip tuntap add tap0 mode tap
$ ip addr add 10.0.0.1/24 dev tap0
$ ip link set tap0 up
$ iptables -P FORWARD ACCEPT
$ iptables -t nat -A POSTROUTING -o venet0 -j MASQUERADE
```

检查`iptables`转发表的`POSTROUTING`链：

```bash
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
```bash
$ iptables -t nat -A PREROUTING -p tcp --dport 22 -j RETURN
```
全部消息转发子虚拟机（子虚拟机的网络地址为`10.0.0.2`）：
```bash
$ iptables -t nat -A PREROUTING -i venet0 -j DNAT --to-destination 10.0.0.2
```

如果不想全部消息都转发子虚拟机，可以这样只将`10000`~`20000`端口的消息转发到子虚拟机：
```bash
$ iptables -t nat -A PREROUTING -i venet0 -p tcp --dport 10000:20000 -j DNAT --to-destination 10.0.0.2
$ iptables -t nat -A PREROUTING -i venet0 -p udp --dport 10000:20000 -j DNAT --to-destination 10.0.0.2
```

甚至，只转发`10221`端口的消息到子虚拟机：

```bash
$ iptables -t nat -A PREROUTING -i venet0 -p tcp --dport 10221 -j DNAT --to-destination 10.0.0.2
$ iptables -t nat -A PREROUTING -i venet0 -p udp --dport 10221 -j DNAT --to-destination 10.0.0.2
```

检查`iptables`转发表的`PREROUTING`链：

```bash
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

```bash
$ iptables -t nat -D PREROUTING 2
```

之后下载91yun.org为我们提供的uml内核（后续我可能会自己编译一个uml，如果有机会的话）：

```bash
#32位系统下载该压缩包
$ wget http://soft.91yun.org/uml/32/uml.tar.xz 
#64位系统下载该压缩包
$ wget http://soft.91yun.org/uml/64/uml.tar.xz 
$ tar xvJf uml.tar.xz
```

进入解压缩后的文件夹，发现里面有两个文件：`vmlinux` (kernel)和`rootfs` (filesystem image)。整个子虚拟机都在`rootfs`内，子虚拟机的网络已经被设置好了。

`rootfs`一开始只有`300M`，为了方便我们在里面安装程序，我们需要先扩大image的容量，这里我扩容到`5GB`：

```bash
$ resize2fs rootfs 5G
```

## 打开user-mode linux子虚拟机

前台运行子虚拟机：（一直不能关闭开启子虚拟机的`ssh`）

```bash
$ ./vmlinux ubda=rootfs eth0=tuntap,tap0 mem=256m
```

> 要想在后台运行子虚拟机：
> `$ nohup ./vmlinux ubda=rootfs eth0=tuntap,tap0 mem=256m >vm.log 2>&1 &`
>
> 这样，log都会被重定向输出到`vmlinux.log`中

其中，`mem=256m`表示分配给子虚拟机`256M`的RAM大小。

等待子虚拟机成功启动，出现如下内容（如果是输出重定向到了`vmlinux.log`中，需要到`vmlinux.log`文件中查看）：

```
Virtual console 3 assigned device '/dev/pts/1'
Virtual console 4 assigned device '/dev/pts/2'
Virtual console 5 assigned device '/dev/pts/3'
Virtual console 6 assigned device '/dev/pts/4'
Virtual console 2 assigned device '/dev/pts/5'
Virtual console 1 assigned device '/dev/pts/6'
```

表明启动完成，其中`/dev/pts/x`就是子虚拟机放`virtual console`的地方，随便记住一个，另开一个`ssh`远程到主机上，用以下命令进入子虚拟机：

```bash
$ screen /dev/pts/1
```

进去后屏幕应该是什么都没有，按一下`Enter`就会出现登陆提示。

> 登陆账户为：root
> 登陆密码为：91yun.org

进入系统后，可以运行一下如下指令：

```bash
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
>
> 杀死当前screen: `Ctrl-a`+`k`

下面，如果你想要令代理程序享受`BBR`算法的加速，你应该让其运行在子虚拟机上。也就是说，将代理的服务端程序安装配置在子虚拟机中，而非原主机上。

> Tips: 如果想要删除所有`iptables`规则: 
> ```Bash
> $ iptables-save | awk '/^[*]/ { print $1 }/^:[A-Z]+ [^-]/ { print $1 " ACCEPT" ; }/COMMIT/ { print $0; }' | iptables-restore 
> ```

