# 部署`Shadowsocks` Client

安装与配置`shadowsocks`步骤与服务器端配置相同，要注意将配置文件中`server`的值设置成你的代理服务器主机的地址。服务器端口要统一，加密方式要统一。例如：

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

本地后台启动`shadowsocks`客户端：

```shell
$ sslocal -c /etc/shadowsocks.json -d start
```

## 本地`Shell`环境变量设置连接`Shadowsocks`

仅仅在后台启动`Shadowsocks`并不能起到什么作用，要想在当前`Shell`中，或者是令本地应用可以通过代理服务器访问外网资源，需要在本地安装配置一个`proxychains`，将`HTTP`请求转成`Socks5`请求，再发送至`shadowsocks`。

本地安装`proxychains`:

```shell
$ apt install proxychains
$ vim /etc/proxychains.conf
```

仅需要修改最后一行为你的shadowsocks代理地址:

```shell
socks5 	127.0.0.1 1080
```

可以查看一下是否成功连接了`Shadowsocks` server:

```shell
$ proxychains curl ip.sb
```

如果返回的不是你当前机器的IP地址，而是代理服务器的IP，恭喜你，代理成功。

之后在想要走代理的程序前加入前缀命令 proxychains 即可，例如:

```bash
$ proxychains bash mytest.sh
$ proxychains docker pull ubuntu:16.04
```

## Ubuntu桌面版pac配置

本地安装`genpac`:

```bash
$ pip install genpac
```

自动生成pac文件:

```bash
$ genpac --proxy="SOCKS5 127.0.0.1:1080" --gfwlist-proxy="SOCKS5 127.0.0.1:1080" -o /etc/autoproxy.pac --gfwlist-url="https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt"
```

进入`System Setting`->`Network`->`Network proxy`, `Method`选择`Automatic`, `Configuration URL`填写刚才生成的pac文件地址:

```bash
file:///etc/autoproxy.pac
```

选择`Apply system wide`, 设置生效