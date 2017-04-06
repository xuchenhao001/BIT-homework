#!/bin/bash

nohup etcd -advertise-client-urls=http://0.0.0.0:4001 \
--listen-client-urls=http://0.0.0.0:4001 &

nohup hyperkube proxy \
--master=http://127.0.0.1:8080 \
--v=2 > 5.log 2>&1 &

nohup hyperkube kubelet \
--api-servers=http://127.0.0.1:8080 \
>2.log 2>&1 &

nohup hyperkube apiserver \
--service-cluster-ip-range=10.0.0.1/24 \
--insecure-port=8080 \
--insecure-bind-address=0.0.0.0 \
--etcd_servers=http://127.0.0.1:4001 \
--v=2 >1.log 2>&1 &

nohup hyperkube controller-manager \
--master=http://127.0.0.1:8080 \
--v=2  >3.log 2>&1 &

nohup hyperkube scheduler \
--master=http://127.0.0.1:8080 \
--v=2  > 4.log 2>&1 &

