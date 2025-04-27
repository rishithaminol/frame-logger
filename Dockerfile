FROM almalinux:9.5 as builder

RUN dnf install 'dnf-command(config-manager)' -y
RUN dnf groupinstall "Development Tools" -y
RUN dnf config-manager --set-enabled crb
RUN dnf install libpcap-devel cmake git git-lfs valgrind -y

WORKDIR /
RUN git clone --recurse-submodules https://github.com/rishithaminol/frame-logger.git
RUN mkdir /frame-logger/build &&\
	cd /frame-logger/build && \
	cmake ../ && \
	make

FROM almalinux:9.5
RUN dnf install libpcap -y
COPY --from=builder /frame-logger/build/frame-logger /usr/local/bin
