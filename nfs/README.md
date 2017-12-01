# Ubuntu install nfs

### server

```bash
$ apt-get install nfs-kernel-server
```

config server share directory (assume is `/nfsserver` ):
```bash
$ echo "/nfsserver *(rw,sync,no_subtree_check,no_root_squash)" >> /etc/exports
$ systemctl restart nfs-kernel-server.service
```

### client

```bash
$ apt install nfs-common
```

configure client mount directory:

```bash
$ echo "<server>:<server-dir>  <local-dir>  nfs  sync,noac" >> /etc/fstab
$ mount -a
```

> replace <server> with your server IP
>
> replace <server-dir> with your server directory (`/nfsserver` in this example)
>
> replace <local-dir> with your local directory

Finished!