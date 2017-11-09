# Deploy IPsec VPN

本教程基于[`strongSwan`](https://strongswan.org/)，一个`IPsec`的开源实现库。
目前广泛应用的VPN协议有以下几种：

* PPTP
* L2TP
* IPsec

基于之前的测试，搭建PPTP的VPN几分钟就会掉线，所以根本没法用。而`IPsec`的实现则较为稳定，可以长期使用。（当然我对其它更多的VPN协议也没有进行充分测试，目前就先写这一个。）纵观网络上的各种文章，都会声明一定是在`KVM`的虚拟化主机上才能用，`OpenVZ`果然是便宜没人权。不能认怂，就是折腾。幸好，我发现我的`OpenVZ`主机是支持`IPsec`的。下面一步步介绍如何在`OpenVZ`虚拟主机上部署`IPsec`协议VPN。

> 注意：如果你的也是`OpenVZ`主机，请在主机供应商的控制页面上寻找是否有`IPsec`支持。如果没有，请不要尝试此教程。`KVM`土豪请随意。

首先下载`strongSwan`的安装包，并安装编译所需的相关依赖：

```bash
$ apt-get update
$ apt-get install libpam0g-dev libssl-dev make gcc
$ wget https://download.strongswan.org/strongswan-5.6.0.tar.gz
$ tar zxf strongswan-5.6.0.tar.gz
$ cd strongswan-5.6.0
```

如果你用的是`OpenVZ`虚拟主机，请运行以下配置：

```bash
$ ./configure --enable-eap-identity --enable-eap-md5 \
--enable-eap-mschapv2 --enable-eap-tls --enable-eap-ttls --enable-eap-peap \
--enable-eap-tnc --enable-eap-dynamic --enable-eap-radius --enable-xauth-eap \
--enable-xauth-pam --enable-dhcp --enable-openssl --enable-addrblock --enable-unity \
--enable-certexpire --enable-radattr --enable-openssl --disable-gmp --enable-kernel-libipsec
```

如果是`KVM`虚拟主机，运行以下配置：

```bash
$ ./configure --enable-eap-identity --enable-eap-md5 \
--enable-eap-mschapv2 --enable-eap-tls --enable-eap-ttls --enable-eap-peap \
--enable-eap-tnc --enable-eap-dynamic --enable-eap-radius --enable-xauth-eap \
--enable-xauth-pam --enable-dhcp --enable-openssl --enable-addrblock --enable-unity \
--enable-certexpire --enable-radattr --enable-openssl --disable-gmp
```

在上面选择适合你的方法配置完成后，我们可以进行编译安装了：

```bash
$ make
$ make install
```

检查以下安装是否成功：

```bash
$ ipsec version
```

如果出现版本信息，则证明安装成功。否则，尝试向`/usr/bin`下做一个软连接即可：

```bash
$ ls -s /usr/local/sbin/ipsec /usr/bin/ipsec
```

下面生成`IPsec`所需的证书：

```bash
$ mkdir certs && cd certs
# 生成CA证书
$ ipsec pki --gen --outform pem > ca.pem
$ ipsec pki --self --in ca.pem --dn "C=com, O=myvpn, CN=VPN CA" --ca --outform pem >ca.cert.pem
# 颁发服务器证书
$ ipsec pki --gen --outform pem > server.pem
$ ipsec pki --pub --in server.pem | ipsec pki --issue --cacert ca.cert.pem \
  --cakey ca.pem --dn "C=com, O=myvpn, CN=202.5.22.22" \
  --san="202.5.22.22" --flag serverAuth --flag ikeIntermediate \
  --outform pem > server.cert.pem
# 颁发客户端证书
$ ipsec pki --gen --outform pem > client.pem
$ ipsec pki --pub --in client.pem | ipsec pki --issue --cacert ca.cert.pem --cakey ca.pem --dn "C=com, O=myvpn, CN=VPN Client" --outform pem > client.cert.pem
# 颁发Windows客户端用证书
$ openssl pkcs12 -export -inkey client.pem -in client.cert.pem -name "client" -certfile ca.cert.pem -caname "VPN CA" -out client.cert.p12
```

安装证书：

```bash
$ cp ca.cert.pem /usr/local/etc/ipsec.d/cacerts/
$ cp server.cert.pem /usr/local/etc/ipsec.d/certs/
$ cp server.pem /usr/local/etc/ipsec.d/private/
$ cp client.cert.pem /usr/local/etc/ipsec.d/certs/
$ cp client.pem  /usr/local/etc/ipsec.d/private/
```

配置IPsec：

```bash
$ cat << EOF > /usr/local/etc/ipsec.conf
config setup
    uniqueids=never 

conn iOS_cert
    keyexchange=ikev1
    # strongswan version >= 5.0.2, compatible with iOS 6.0,6.0.1
    fragmentation=yes
    left=%defaultroute
    leftauth=pubkey
    leftsubnet=0.0.0.0/0
    leftcert=server.cert.pem
    right=%any
    rightauth=pubkey
    rightauth2=xauth
    rightsourceip=10.31.2.0/24
    rightcert=client.cert.pem
    auto=add

conn android_xauth_psk
    keyexchange=ikev1
    left=%defaultroute
    leftauth=psk
    leftsubnet=0.0.0.0/0
    right=%any
    rightauth=psk
    rightauth2=xauth
    rightsourceip=10.31.2.0/24
    auto=add

conn networkmanager-strongswan
    keyexchange=ikev2
    left=%defaultroute
    leftauth=pubkey
    leftsubnet=0.0.0.0/0
    leftcert=server.cert.pem
    right=%any
    rightauth=pubkey
    rightsourceip=10.31.2.0/24
    rightcert=client.cert.pem
    auto=add

conn windows7
    keyexchange=ikev2
    ike=aes256-sha1-modp1024! 
    rekey=no
    left=%defaultroute
    leftauth=pubkey
    leftsubnet=0.0.0.0/0
    leftcert=server.cert.pem
    right=%any
    rightauth=eap-mschapv2
    rightsourceip=10.31.2.0/24
    rightsendcert=never
    eap_identity=%any
    auto=add
EOF
```

配置`strongSwan`：

```bash
$ cat << EOF > /usr/local/etc/strongswan.conf
# strongswan.conf - strongSwan configuration file
charon {
    load_modular = yes
    duplicheck.enable = no
    compress = yes
    
    dns1 = 8.8.8.8
    dns2 = 8.8.4.4
    
    # for Windows only
    nbns1 = 8.8.8.8
    nbns2 = 8.8.4.4
    plugins {
        include strongswan.d/charon/*.conf
    }
}
include strongswan.d/*.conf
```

配置登陆密钥：

```bash
$ cat << EOF > /usr/local/etc/ipsec.secrets
: RSA server.pem
: PSK "<mypsk>"
<username> %any : EAP "<password>"
```

> 注意：将<mypsk>替换成你自定义的PSK (pre-shared key)密钥。将<username>和<password>换成你登陆VPN所用的账号和密码。如果想要配置多账号登陆，可以以上面的格式重复写n个账号/密码，或是PSK。

开启`iptables`转发：

```bash
# 接受500端口以及4500端口udp输入
$ iptables -A INPUT -p udp --dport 500 -j ACCEPT
$ iptables -A INPUT -p udp --dport 4500 -j ACCEPT
# 转发流量定向
$ iptables -t nat -A POSTROUTING -s 10.31.2.0/24 -o [your-ethernet-dev] -j MASQUERADE
$ iptables -A FORWARD -s 10.31.2.0/24 -j ACCEPT
$ echo 1 > /proc/sys/net/ipv4/ip_forward
```

> 注意：将[your-ethernet-dev]替换成你的网卡设备名称，比如`eth0`。如果不知道的话，可以用命令`ifconfig`查看，其中具有你的VPS公网IP的一项便是。

最后开启`IPsec`服务：

```bash
$ ipsec start
```

## 客户端连接

### Windows

想办法将上面`certs`文件夹下的`client.cert.p12`证书文件下载到本地。点开`开始菜单->运行`，键入`certlm.msc`，进入`受信任的根证书颁发机构`项的子项`证书`，在树形目录`证书`上单击右键，`所有任务->导入`，选择我们下载的`client.cert.p12`证书并导入。

之后进入`网络和Internet`设置，添加VPN连接，VPN类型选择`IKEv2`，用户名和密码填写上面定义的<username>和<password>即可。

### iOS/Android/Mac OS X

打开设置，找到VPN，选择`IPSec Xauth PSK`方式，服务端IP填写你的VPS公网IP，预共享密钥填写你上面设定的<mypsk>(不包括引号)，用户名密码也是你上面设置的<username>和<password>。

WP8.1手机安装ca.cert.pem，进入设置-VPN添加IKEv2连接，地址为证书中的地址或IP，通过用户名-密码连接。 
Windows连接也是一样，但注意将证书导入本地计算机而不是当前用户的“受信任的证书颁发机构”。 
