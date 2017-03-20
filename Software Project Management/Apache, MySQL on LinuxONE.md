# Apache, Mysql on LinuxONE(s390x)

##Environment
默认创建一个RHEL 7.2实例<br/>
转换为root用户方便接下来操作：
		sudo -i
查看磁盘使用信息：
		df -h
/dasda2空间只有5.8G，为了防止空间不足，将/root软连接到/dasdb1/data：
		 mv /root /data
		 ln -s /data/root /root
挂载成功。<br/>

## Building Apache HTTP Server
首先安装一些基本包
		yum install git openssl openssl-devel gcc libtool autoconf make pcre pcre-devel libxml2 libxml2-devel expat-devel which wget tar
我默认在/root下进行下载安装操作（可以改）。下载apache的包：
		git clone https://github.com/apache/httpd.git
		cd httpd
		git checkout 2.4.25
		cd srclib
		git clone https://github.com/apache/apr.git
编译Apache Portable Runtime：
		cd apr
		./buildconf
此处，生成的configure有一些小问题，需要更改一下其中的问题：
		vi configure
利用vi编辑configure文件，进入后，在命令方式下，输入：
		/RM
按回车。表示在configure中搜索"RM"关键词，按"N"向上搜索，"n"向下搜索。在这里我们使用向上搜索。连续按4次"N"之后，可以看到如下文本：
		RM='$RM'


## Reference
https://github.com/linux-on-ibm-z/docs/wiki/Building-Apache-HTTP-Server<br/>