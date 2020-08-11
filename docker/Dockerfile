FROM ubuntu:20.04

RUN apt update && \
    apt install -y gcc cmake make gcc-mingw-w64 && \
    rm -rf /var/lib/apt/lists/*

# RUN update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix

RUN apt update && \
    apt install -y wine && \
    rm -rf /var/lib/apt/lists/*

RUN winecfg
