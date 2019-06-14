# 部署`Shadowsocks` Server

首先更新一下数据源，升级一下软件包：

```
apt-get update
apt-get upgrade
```

安装`python`和`pip`：

```
$ apt install python python-pip python-setuptools git
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

此时已经在后台成功启动了`shadowsocks`，并且监听`10221`端口。

服务端配置完成后，可以开始配置客户端，请参考[Shadowsocks-Client](https://github.com/xuchenhao001/BIT-homework/blob/master/Proxy/shadowsocks/Shadowsocks-Client.md)一文。
