# `Ubuntu`环境下本地应用代理配置

代理搭建好之后，为了能够服务广大科研工作者，`Linux`环境下我们经常需要配置一些应用的代理，往往这些配置又没有一个统一的规范。故在这里我进行一下简单的整理和总结。

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
