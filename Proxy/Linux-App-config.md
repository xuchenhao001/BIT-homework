# `Ubuntu`环境下本地应用代理配置

代理搭建好之后，为了能够服务广大科研工作者，`Linux`环境下我们经常需要配置一些应用的代理，往往这些配置又没有一个统一的规范。故在这里我进行一下简单的整理和总结。

## Shell

如果你想在`Shell`里走`shadowsocks`代理：（其中`8118`就是本地`privoxy`的默认代理端口，本地`shadowsocks`的默认代理端口是`1080`）

```bash
$ export http_proxy='http://localhost:8118'
$ export https_proxy='http://localhost:8118'
```

可以查看一下是否成功连接了`Shadowsocks` server：

```bash
$ curl ip.sb
```

如果返回的不是你当前机器的IP地址，而是代理服务器的IP，恭喜你，代理成功。

## git

常常我们在国内用`git`再拉数据或者上传数据的时候速度非常慢，如果想要配置`git`的`shadowsocks`代理，可以这样设置：

```shell
$ export all_proxy=socks5://localhost:1080
```

## Docker

用`Docker`拉国外的镜像时，也常常是不给力，所以若想让`Docker`走`shadowsocks`代理，可以按如下方式配置：

```shell
$ mkdir -p /etc/systemd/system/docker.service.d
$ vi /etc/systemd/system/docker.service.d/http-proxy.conf
```

在这个新建的配置文档中加入以下语句，如果你使用了我写的`privoxy`配置方式做了`http`转`socks5`代理，这里可以填写`privoxy`的`8118`端口：

```shell
[Service]
Environment="HTTP_PROXY=http://localhost:8118/" "HTTPS_PROXY=http://localhost:8118/"
```

注意，`localhost`不能用`127.0.0.1`替代。

配置完成后`Docker`重载配置文件，重启`Docker`：

```shell
$ systemctl daemon-reload
$ systemctl restart docker
```

之后就可以从`gcr`等库中拉镜像了。

## npm

配置npm代理，就不需要每次都安装淘宝的`cnpm`了，这里仍以`privoxy`的`8118`端口为例：

```shell
$ npm config set proxy localhost:8118
$ npm config set https-proxy localhost:8118
```
