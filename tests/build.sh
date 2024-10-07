#/bin/bash

docker build -t my_ping -f my_ping . && \
	docker build -t origin_ping -f origin_ping .  && \
	docker compose up -d
