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

仅仅在后台启动`Shadowsocks`并不能起到什么作用，要想在当前`Shell`中，或者是令本地应用可以通过代理服务器访问外网资源，需要在本地安装配置一个`privoxy`，将`HTTP`请求转成`Socks5`请求，再发送至`shadowsocks`。大部分`Server`上的应用都是支持使用`HTTP`代理的，但是麻烦的地方在于不同的软件，我们需要不同的配置方式。

本地安装`privoxy`将`HTTP`消息代理为`Socks5`：

```shell
$ apt install privoxy
$ vim /etc/privoxy/config
```

去掉这一行的注释：

```shell
forward-socks5t   /               127.0.0.1:1080 .
```

开启`privoxy`代理：

```shell
$ service privoxy start 
```

现在，如果你想在`Shell`里走`socks5`代理：（其中`8118`就是本地`privoxy`的默认代理端口，本地`shadowsocks`的默认代理端口是`1080`）

```shell
$ export http_proxy='http://localhost:8118'
$ export https_proxy='https://localhost:8118'
```

可以查看一下是否成功连接了`Shadowsocks` server：

```shell
$ curl ip.sb
```

如果返回的不是你当前机器的IP地址，而是代理服务器的IP，恭喜你，代理成功。
