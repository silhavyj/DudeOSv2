FROM debian:stable-slim

RUN \
	apt-get update && \
	apt-get install -y \
	nasm xxd build-essential && \
	apt-get autoremove -y && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists 

WORKDIR /root/dudeosv2
CMD make
