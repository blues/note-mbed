# Copyright 2022 Blues Inc.  All rights reserved.
# Use of this source code is governed by licenses granted by the
# copyright holder including that found in the LICENSE file.

# Build development environment
# build --tag note-mbed .

# Launch development environment
# docker run --entrypoint bash --interactive --rm --tty --volume "$(pwd)":/host-volume/ --workdir /host-volume/ note-mbed

# Define global arguments
ARG DEBIAN_FRONTEND="noninteractive"
ARG UID=1000
ARG USER="blues"

# POSIX compatible (Linux/Unix) base image
FROM debian:stable-slim

# Import global arguments
ARG DEBIAN_FRONTEND
ARG UID
ARG USER

# Define local arguments
ARG BINDIR=/usr/local/bin
ARG ECHO_BC_FILE='$bcfile'

# Create Non-Root User
RUN ["dash", "-c", "\
    addgroup \
     --gid ${UID} \
     \"${USER}\" \
 && adduser \
     --disabled-password \
     --gecos \"\" \
     --ingroup \"${USER}\" \
     --uid ${UID} \
     \"${USER}\" \
 && usermod \
     --append \
     --groups \"dialout,plugdev\" \
     \"${USER}\" \
"]

# Establish development environment
RUN ["dash", "-c", "\
    apt-get update --quiet \
 && apt-get install --assume-yes --no-install-recommends --quiet \
     bash \
     bash-completion \
     bzip2 \
     ca-certificates \
     curl \
     gcc \
     git \
     make \
     mercurial \
     python-is-python3 \
     python3 \
     python3-dev \
     python3-pip \
     python3-venv \
     ssh \
     tree \
     wget \
 && apt-get clean \
 && apt-get purge \
 && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* \
"]

RUN wget -O archive.tar.bz2 "https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2?revision=108bd959-44bd-4619-9c19-26187abf5225&la=en&hash=E788CE92E5DFD64B2A8C246BBA91A249CB8E2D2D" && \
    echo fe0029de4f4ec43cf7008944e34ff8cc archive.tar.bz2 > archive.md5 && md5sum -c archive.md5 && rm archive.md5 && \
    tar xf archive.tar.bz2 -C /opt && \
    rm archive.tar.bz2 && \
    apt-get autoclean -y && \
    apt-get autoremove -y && \
    apt-get clean

ENV PATH=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH

RUN ["dash", "-c", "\
    python3 -m pip install virtualenv && \
    python3 -m venv /venv && \
    . /venv/bin/activate"]

# fix version of markupsafe - https://github.com/aws/aws-sam-cli/issues/3661
# need intelhex 2.3.0, but not available in PyPI https://github.com/adafruit/Adafruit_nRF52_Arduino/pull/597
RUN ["dash", "-c", "\
    python3 -m pip install -U \
        mbed-cli \
        markupsafe==2.0.1 \
        intelhex==2.3.0 \
    "]

RUN ["dash", "-c", "\
    mkdir /mbed-ci && \
    cwd=$(pwd) && \
    cd / && \
    mbed new mbed-ci && \
    cd /mbed-ci/mbed-os && \
    mbed update mbed-os-5.15.8 && \
    cd $cwd \
    "]

RUN ["dash", "-c", "\
    mbed config -G GCC_ARM_PATH /opt/gcc-arm-none-eabi-9-2019-q4-major/bin \
    "]

COPY compile.sh /entrypoint.sh

WORKDIR /host-volume
ENTRYPOINT ["/entrypoint.sh"]
CMD ["help"]
