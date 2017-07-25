# 本地Ubuntu Server连接`shadowsocks`代理

安装与配置`shadowsocks`步骤与上面服务器端配置相同，要注意将配置文件中`server`的值设置成你的OpenVZ主机地址。服务器端口要统一，加密方式一定要统一。例如：

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

## 本地`Shell`环境变量设置连接`shadowsocks`

仅仅在后台启动`shadowsocks`并不能起到什么作用，要想在`shell`命令中可以访问代理，需要本地配置一个`privoxy`将`http`请求转成`socks5`请求，再发送至`shadowsocks`即可。大部分`Server`上的应用都是支持使用代理的，故大部分软件都是可以通过`shadowsocks`上外网的。但是麻烦的地方在于针对不同软件，我们需要不同的配置方式。

本地安装`privoxy`将http消息代理为`socks5`：

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

现在，如果你想在`terminal`里走`socks5`代理：（其中`8118`就是`privoxy`的默认代理端口，本地`shadowsocks`的代理端口是`1080`）

```shell
$ export http_proxy='http://localhost:8118'
$ export https_proxy='https://localhost:8118'
```

可以查看一下是否成功连接了`shadowsocks server`：

```shell
$ curl ip.gs
```

如果返回的不是你当前机器的IP地址，而是国外的IP，恭喜你，代理成功。

## git

常常我们在国内用`git`再拉数据或者上传数据的时候速度非常慢，如果想要让`git`走`shadowsocks`代理，可以这样设置：

```shell
$ export all_proxy=socks5://localhost:1080
```

## Docker

用`Docker`拉国外的镜像时，也常常是不给力，所以若想让`Docker`走`shadowsocks`代理，可以按如下方式配置：

```shell
$ mkdir -p /etc/systemd/system/docker.service.d
$ vi /etc/systemd/system/docker.service.d/http-proxy.conf
```

在这个新建的配置文档中加入以下语句，如果你上面使用了我写的`privoxy`配置方式做了`http`转`socks5`代理，这里可以填写：

```shell
[Service]
Environment="HTTP_PROXY=http://localhost:8118/" "HTTPS_PROXY=https://localhost:8118/"
```

注意，`localhost`不能用`127.0.0.1`替代。

配置完成后`Docker`重载配置文件，重启`Docker`：

```shell
$ systemctl daemon-reload
$ systemctl restart docker
```

之后就可以从gcr等库中拉镜像了。

## npm

配置npm代理，就不需要每次都安装淘宝的`cnpm`了：

```shell
$ npm config set proxy http://localhost:8118
$ npm config set https-proxy http://localhost:8118
```
