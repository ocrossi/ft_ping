FROM debian:latest

RUN apt-get update -yq && apt-get upgrade -yq && \
	apt-get install git inetutils-ping \
	make gcc sudo libcap2-bin neovim tshark -yq 


WORKDIR /ft_ping
COPY . .
