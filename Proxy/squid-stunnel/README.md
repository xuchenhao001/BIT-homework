# `Squid`-`Stunnel`代理

比`shadowsocks`更为简单的代理方案是`squid`+`stunnel`代理。其中服务端部署运行`squid`反向代理以缓冲访问的数据，比如`Google`。本地端部署`stunnel`用以加密与`squid`之间的通信，使得访问更加安全，且不会被墙掉。

## 服务端`Squid`安装

### 证书生成

既然要加密`stunnel`与`squid`之间的通信，就必须要为`squid`颁发证书。这里我们利用`openssl`工具自己颁发一个即可。

```bash
$ mkdir -p /root/.squid
$ openssl req -nodes -newkey rsa:2048 -keyout /root/.squid/cert.key -out /root/.squid/cert.csr -subj "/C=/ST=/L=/O=/OU=/CN=squid"
$ openssl x509 -req -sha256 -days 365 -in /root/.squid/cert.csr -signkey /root/.squid/cert.key -out /root/.squid/cert.crt
```

### 安装

本来打算直接利用`apt install squid`来安装`squid`，但是发现`squid`的发行包里并不支持加密模块，所以我们需要自己编译`squid`。首先下载`squid`源码包：

```bash
$ wget http://www.squid-cache.org/Versions/v3/3.5/squid-3.5.27.tar.gz
```

解压缩：

```bash
$ tar -zxf squid-3.5.27.tar.gz
$ cd squid-3.5.27/
```

编译前，需要安装两个负责加密的依赖包：

```bash
$ apt install -y autoconf g++ make openssl libssl-dev
```

编译，这里一定要加入`--with-openssl`选项，使得`squid`支持加密连接：

```bash
$ ./configure --with-openssl
$ make all
```

> 稍等片刻，可以去喝杯茶了

安装：

```bash
$ make install
```

安装成功后，在`/usr/bin`下制作一个软连接以方便使用`squid`：

```bash
$ ln -s /usr/local/squid/sbin/squid /usr/bin/squid
```

配置`squid.conf`：

```bash
$ sed -i "1,/cache_dir ufs/d" /usr/local/squid/etc/squid.conf
$ sed -i "1i\https_port 443 cert=\/root\/.squid\/cert.crt key=\/root\/.squid\/cert.key" /usr/local/squid/etc/squid.conf
$ sed -i "1i\http_access allow all" /usr/local/squid/etc/squid.conf
```

更改`log`文件夹的权限，允许`squid`程序写入：

```bash
$ chmod 777 /usr/local/squid/var/logs/
```

最后，启动`squid`：

```bash
$ squid
```

如果没报错的话表明后台运行`squid`成功。同时可以用下面命令查看进程：

```bash
$ ps -ef|grep squid
```

如果进程存在，表明服务端配置成功。

> 注意防火墙要允许`443`端口访问。可简单使用`iptables -F`命令清空防火墙规则链。

## 客户端`Stunnel`安装

服务端安装`stunnel`很简单，只需要安装好后更改一下配置，即可加密连接服务端`squid`，实现远程代理上网。

## Windows

官网提供了`Windows`环境下的安装包。

```
https://www.stunnel.org/downloads.html
```

选择相应的`win32`的安装程序，下载完成后按照提示进行安装即可。

初次运行，需要更改配置，双击右下角`stunnel`程序后，选择`Configuration`->`Edit Configuration`，将配置文件替换成如下内容：

```
[https]
client = yes
accept = 127.0.0.1:8088
connect = [your.server.ip]:443
```

> 其中`accept`字段内容为本地服务端口，这里我默认使用了`8088`端口。也就是说，如果想要本地浏览器通过代理上网的话，应该配置`HTTP`代理为`127.0.0.1:8088`，其他应用同理
>
> 其中`connect`字段内容为服务端地址和端口。需要将`[your.server.ip]`更改为你服务器端的IP地址。

配置好之后，选择`Configuration`->`Reload Configuration`重载配置，现在就可以使用搭建的`squid`-`stunnel`代理上网了。

## Linux

各个发行版的源下几乎都有`stunnel`的身影。以`Ubuntu`为例，安装`stunnel`：

```bash
$ apt install stunnel4
```

修改配置文件为以下内容：

```bash
$ vim /etc/stunnel/stunnel.conf
[https]
client = yes
accept = 127.0.0.1:8088
connect = [your.server.ip]:443
```

具体配置说明同`Windwos`。

配置开启`stunnel`：

```bash
$ vim /etc/default/stunnel4
```

将其中的`ENABLED=0`改为`ENABLED=1`即可。

之后重启`stunnel`服务：

```bash
$ systemctl restart stunnel4.service
```

修改环境变量使`bash`通过`stunnel`代理上网：

```bash
$ export http_proxy='http://localhost:8088'
$ export https_proxy='https://localhost:8088'
```

测试是否连接成功：

```bash
$ curl ip.sb
```

如果返回结果是`Squid`服务器的地址，证明代理成功。