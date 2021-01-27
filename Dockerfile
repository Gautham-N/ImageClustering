# Minimal docker container to build project
# Image: rabits/qt:5.13-desktop

FROM ubuntu:18.04
MAINTAINER Gautham 

# Install updates & requirements:
#  * git, openssh-client, ca-certificates - clone & build
#  * locales, sudo - useful to set utf-8 locale & sudo usage
#  * curl - to download Qt bundle
#  * cmake, ninja-build - build system
#  * build-essential, pkg-config, libgl1-mesa-dev - basic Qt build requirements
#  * libsm6, libice6, libxext6, libxrender1, libfontconfig1, libdbus-1-3 - dependencies of the Qt bundle run-file
#  * cmake - useful for developers

RUN apt update && apt full-upgrade -y && apt install -y --no-install-recommends \
    git \
    openssh-client \
    ca-certificates \
    locales \
    sudo \
    curl \
    cmake \
    ninja-build \
    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    libdbus-1-3 \
    && apt-get -qq clean \
    && rm -rf /var/lib/apt/lists/*

RUN apt update

RUN apt-get install -y qt5-default

RUN apt-get install -y qtcreator

RUN apt install -y build-essential

RUN apt install -y wget



# Reconfigure locale
RUN locale-gen en_US.UTF-8 && dpkg-reconfigure locales

# Add group & user + sudo
RUN groupadd -r user && useradd --create-home --gid user user && echo 'user ALL=NOPASSWD: ALL' > /etc/sudoers.d/user

USER user

WORKDIR /home/user

# OPENCV


RUN wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=16P3Ck7X5WHrDaCpxM-fhpmUXgcoEEeI7' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=16P3Ck7X5WHrDaCpxM-fhpmUXgcoEEeI7" -O DockerDir1.tar.xz && rm -rf /tmp/cookies.txt

USER root

RUN tar -xf DockerDir1.tar.xz



#WORKDIR /home/user

#ENV HOME /home/user

USER root

RUN cp -R /home/user/DockerDir1/include /usr/local/

RUN cp -R /home/user/DockerDir1/lib /usr/local/

RUN cp -R /home/user/DockerDir1/Cluster.conf /etc/ld.so.conf.d/

RUN ldconfig




