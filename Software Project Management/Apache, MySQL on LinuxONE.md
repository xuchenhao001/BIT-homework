# Apache, Mysql on LinuxONE(s390x)

这是我写的第一份文档。在这次软件项目管理课上，我负责写我们`Day`项目的后台部分。我选择了`Python`，使用了`Tornado`框架，并且尝试在IBM Community Cloud的免费LinuxONE上搭建服务器。以下是我的环境搭建说明。

## Environment

默认创建一个`RHEL 7.2`实例，使用`root`用户方便接下来操作：

    sudo -i  

查看磁盘使用信息：

    df -h

`/dasda2`空间只有5.8G，为了防止空间不足，将`/root`软连接到`/dasdb1/data`：

    mv /root /data
    ln -s /data/root /root

挂载成功。  

## Building Apache HTTP Server
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

经过许久的摸索，发现是LinuxONE防火墙搞的鬼，我们需要关闭防火墙（重启失效）：

	service iptables stop

之后可以在本地浏览器中打开LinuxONE实例的地址，我们可以看到一个大大的

	It works!

出现在浏览器页面上方，至此，Apache HTTP Server已经搭建成功。

>如果你想关闭Apache HTTP Server，使用以下命令即可：

	/usr/local/apache2/bin/apachectl -k stop

## Building MySQL

首先安装一些必须的程序，其中主要是gcc编译器以对`MySQL-server`源码进行编译：

	yum install git gcc gcc-c++ make cmake bison ncurses-devel perl-Data-Dumper

让我们回到`/root`目录，并下载`MySQL-server`源码：

	cd ~
	git clone https://github.com/mysql/mysql-server.git

下载完毕后，进入`mysql-server`目录，并检出`5.7.17`版本：

	cd mysql-server
	git checkout tags/mysql-5.7.17

配置并编译`MySQL`：

	cmake . -DDOWNLOAD_BOOST=1 -DWITH_BOOST=.
	gmake

>**提示：**此处时间较长，需要耐心等待

（可选）运行单元测试：

	gmake test

安装编译后的MySQL程序：

	gmake install

>**提示：**此处时间较长，需要耐心等待

正常情况下，利用软件包安装的MySQL不需要自己进行配置，但是这里的情况比较复杂，二进制文件自行编译安装之后，需要我们自己进行一系列初始化配置。

首先找到`MySQL`的安装目录，默认是`/usr/local/mysql`：

	cd /usr/local/mysql

创建一个目录，为了安全导入/导出操作：

	mkdir mysql-files
	chmod 750 mysql-files

创建`MySQL`用户、用户组，并为`MySQL`授予权限：

	groupadd mysql
	useradd -r -g mysql mysql
	chown -R mysql .
	chgrp -R mysql .

初始化数据目录，包括`MySQL`的授权表等：

	bin/mysqld --initialize --user=mysql

注意，执行完此步骤`Terminal`会返回一个随机密码，例如：

	[Note] A temporary password is generated for root@localhost: wvItoD!02qGC

此时应记下`MySQL`的`mysql`账户初始密码，比如这里是`wvItoD!02qGC`。

给`root`账户赋予权限，而数据目录和`mysql-files`目录必须由`mysql`拥有：

	chown -R root .
	chown -R mysql data mysql-files

运行之前，需要更改一下守护进程的默认`socket`路径：

	vi /etc/my.cnf

将其中定义`socket`的一行改为：

	socket=/tmp/mysql.sock

键入`wq`保存并退出，最后，运行`MySQL`服务：

	/usr/local/mysql/bin/mysqld --user=mysql

>如果想要关闭`MySQL`服务，最简单方式：

	killall mysqld

开启`MySQL`服务后，可以用`root`账户打开一个`MySQL`监视器以操纵数据库：

	/usr/local/mysql/bin/mysql -u root -p

此时需要输入`root`账户密码，就是上方`MySQL`自动生成给我们的那个密码。

但是登陆之后会出现错误如下：

	ERROR 1820 (HY000): You must reset your password using ALTER USER statement before executing this statement.

意思是需要我们输入新的密码，才能使用`MySQL`。比如，新密码设为`myPassword`：

	SET PASSWORD = PASSWORD('myPassword');

之后就可以正常地使用数据库了。

## Reference
https://github.com/linux-on-ibm-z/docs/wiki/Building-Apache-HTTP-Server  
https://github.com/linux-on-ibm-z/docs/wiki/Building-MySQL  
https://dev.mysql.com/doc/refman/5.7/en/postinstallation.html