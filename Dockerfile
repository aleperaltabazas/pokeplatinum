FROM ubuntu:24.04 AS build
ENV DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture i386 && apt-get update && apt-get install -y \
    bison flex g++ gcc-arm-none-eabi git make ninja-build pkg-config wget curl ca-certificates \
    python3 xz-utils nasm libc6:i386 libpng-dev unzip && rm -rf /var/lib/apt/lists/*
RUN git config --global --add safe.directory '*'
WORKDIR /work
COPY . .
ARG ROM_REVISION=0
RUN ROM_REVISION=${ROM_REVISION} make rom

# Export only the ROM:
#   docker build --output type=local,dest=out .
FROM scratch
COPY --from=build /work/build/pokeplatinum.us.nds /pokeplatinum.us.nds
