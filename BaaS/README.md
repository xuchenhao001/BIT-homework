# Kubernetes 1.5.2 Deploy on Local Ubuntu 16.04

## Prerequisite

* kubectl `1.5.2`
* hyperkube `1.5.2`
* etcd `3`

## Guide
Login with root

	sudo -i

Update other packages

	apt get update
	apt get upgrade

Install docker

	apt install docker.io

Move your binary file `kubectl`, `hyperkube` and `etcd` to `/usr/local/bin/` for convenience

	chmod +x kubectl
	chmod +x hyperkube
	chmod +x etcd
	mv kubectl /usr/local/bin/
	mv hyperkube /usr/local/bin/
	mv etcd /usr/local/bin/

You can use start.sh for quick deploy.

	

check daemon process

	ps -aux | grep {process_name}

images to docker

	docker load -i {tar_name}

images deploy

	kubectl run --image=nginx nginx-app --port=80 --env="DOMAIN=cluster"

expose ports

	kubectl expose deployment kubernetes-dashboard --port=80 --name=dashboard-http