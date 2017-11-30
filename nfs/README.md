# Ubuntu install nfs

### server

```bash
$ apt-get install nfs-kernel-server
```

config server share directory (assume is `/tmp` ):
```bash
$ echo "/tmp *(rw,sync,no_subtree_check)" >> /etc/exports
```

### client

```bash
$ apt install nfs-common
```

configure client mount directory:

```bash
$ echo "<server>:<server-dir>	<local-dir>	nfs	rw	0	0" >> /etc/fstab
$ mount -a
```

> replace <server> with your server IP
>
> replace <server-dir> with your server directory (`/tmp` in this example)
>
> replace <local-dir> with your local directory

Finished!