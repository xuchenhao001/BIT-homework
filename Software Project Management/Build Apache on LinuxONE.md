# Apache on LinuxONE(s390x)

软件项目管理课上，我负责写我们`Day`项目的后台部分。我选择了`Python`，使用了`Tornado`框架，并且尝试在IBM Community Cloud的免费LinuxONE上搭建服务器。机缘巧合，研究了一下Apache在LinuxONE上的安装。

## Environment

默认创建一个`RHEL 7.2`实例，使用`root`用户方便接下来操作：

    sudo -i  

查看磁盘使用信息：

    df -h

`/dasda2`空间只有5.8G，为了防止空间不足，将`/root`软连接到`/dasdb1/data`：

    mv /root /data
    ln -s /data/root /root

挂载成功。  

## Build Apache HTTP Server

首先安装一些基本包

    yum install git openssl openssl-devel gcc libtool autoconf make pcre pcre-devel libxml2 libxml2-devel expat-devel which wget tar

我默认在`/root`下进行下载安装操作（可以更改）。下载`Apache`的包：

    git clone https://github.com/apache/httpd.git
    cd httpd
    git checkout 2.4.25
    cd srclib
    git clone https://github.com/apache/apr.git

编译Apache Portable Runtime(APR)：

    cd apr
    ./buildconf

此处，生成的`configure`文件有一些小问题，需要更改一下其中的问题：

    vi configure

利用`vi`编辑`configure`文件，进入后，在命令方式下，输入：

    /RM

按回车。表示在`configure`中搜索`RM`关键词，按`N`向上搜索，`n`向下搜索。在这里我们使用向上搜索。连续按4次`N`之后，可以看到如下文本：

    RM='$RM'

键入`i`切换为插入模式，将上面语句更改为：

	RM='$RM -f'

使其删除操作均为强制。之后键入`ESC`切换回命令模式，键入：

	:wq

保存并退出。下面继续安装`apr`：

	./configure
	make
	make install
	
下面编译并安装Apache HTTP Server：

	cd ~/httpd
	./buildconf
	./configure
	make
	make install

如果以上操作均按此完成，我们已经装好了Apache HTTP Server了，在`/usr/local/apache2`目录下。

下面，我们将Apache HTTP Server开启：

	cd /usr/local/apache2
	bin/apachectl configtest
	bin/apachectl -k start



此时Apache HTTP Server守护进程已经开启，但是外网仍无法访问。

经过许久的摸索，发现是LinuxONE防火墙搞的鬼，我们需要更改防火墙的端口开放规则：

	vi /etc/sysconfig/iptables

在开放端口`22`的规则下面写一条开放端口80的规则如下：

	-A INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT

重启防火墙服务：

	service iptables restart

之后可以在本地浏览器中打开LinuxONE上虚拟主机的地址，我们可以看到一个大大的

	It works!

出现在浏览器页面上方，至此，Apache HTTP Server已经搭建成功。

>如果你想关闭Apache HTTP Server，使用以下命令即可：

	/usr/local/apache2/bin/apachectl -k stop

## Reference
https://github.com/linux-on-ibm-z/docs/wiki/Building-Apache-HTTP-Server