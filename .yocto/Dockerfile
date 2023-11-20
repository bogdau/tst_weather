FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update



###############################################################################
###############################################################################
#                             Common dependencies                             #
###############################################################################
###############################################################################

# Install Poky dependencies
RUN apt-get install -y gawk wget git-core diffstat unzip texinfo gcc-multilib \
    build-essential chrpath socat cpio python3 python3-pip python3-pexpect \
    xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa \
    libsdl1.2-dev xterm python3-subunit mesa-common-dev rsync zstd file \
    liblz4-tool

# Install common utils
RUN apt-get install -y git zip nano screen tree curl sudo flex bison libncurses-dev tzdata graphviz

RUN mkdir -p /opt/repo
RUN curl https://storage.googleapis.com/git-repo-downloads/repo > /opt/repo/repo
RUN chmod a+rx /opt/repo/repo

# Set utf-8 locale for Poky
RUN apt-get install -y locales
RUN sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && \
    locale-gen
ENV LANG en_US.UTF-8  
ENV LANGUAGE en_US:en  
ENV LC_ALL en_US.UTF-8

# Fix timezone (/etc/localtime) config
ENV TZ 'Europe/Kyiv'
RUN echo $TZ > /etc/timezone && \
    rm /etc/localtime && \
    ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

# RUN useradd -ms /bin/bash pokyuser
#RUN adduser --disabled-password --gecos '' pokyuser
#RUN adduser pokyuser sudo
#RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers


ARG host_uid=1001
ARG host_gid=1001
RUN groupadd -g 1000 dev \
            && useradd -u 1000 -g dev -d /home/dev dev \
            && mkdir /home/dev \
            && chown -R dev:dev /home/dev
 ###############################################################################
###############################################################################
#                           Yocto general dependencies                        #
###############################################################################
###############################################################################

# COPY .yocto/3rdparty /3rdparty
# WORKDIR /3rdparty/pipoe-issue-20
# RUN python3 setup.py install

ENV YOCTO_RELEASE=kirkstone

# RUN mkdir /poky && chown pokyuser:pokyuser /poky

# RUN mkdir /downloads && chown pokyuser:pokyuser /downloads

# USER pokyuser
# # Set default git credentials
RUN git config --global user.name "pokyuser"
RUN git config --global user.email "pokyuser@example.com"

WORKDIR /
RUN rm -rf poky/
# It is redundand, due to NXP repo contains pinned version of poky as submodule
RUN git clone git://git.yoctoproject.org/poky -b $YOCTO_RELEASE
# RUN pip3 install -r /poky/bitbake/toaster-requirements.txt

WORKDIR /poky
RUN mkdir /poky/build

# COPY poky/meta-custom /poky/
# It is redundand, due to NXP repo contains pinned version of meta-openembedded as submodule
#RUN git clone git://git.openembedded.org/meta-openembedded -b $YOCTO_RELEASE

RUN git clone https://github.com/agherzan/meta-raspberrypi -b $YOCTO_RELEASE
RUN git clone https://github.com/openembedded/meta-openembedded.git -b $YOCTO_RELEASE
RUN git clone https://github.com/meta-qt5/meta-qt5.git -b $YOCTO_RELEASE
RUN git clone https://github.com/aws4embeddedlinux/meta-aws.git -b $YOCTO_RELEASE
COPY meta-custom /poky/meta-custom
COPY meta-image-config /poky/meta-image-config
COPY conf /poky/build/conf
COPY custom-images/ /poky/custom-images

RUN chown -R dev:dev /poky

# USER dev
WORKDIR /poky/

#RUN sudo rm -rf /poky/build/conf 
#RUN sudo ln -sf /poky/build/conf         /poky/build/conf
#WORKDIR /poky/build
#RUN bash -c "bitbake core-image-minimal-xfce"
# RUN bitbake core-image-minimal-xfce




# RUN bitbake core-image-minimal-xfce

# ###############################################################################
# ###############################################################################
# #                         Project-related dependencies                        #
# ###############################################################################
# ###############################################################################

# #   It is redundand for this project due to NXP repo contains pinned version 
# # of meta-openembedded as submodule
# # RUN git clone https://github.com/meta-qt5/meta-qt5.git -b $YOCTO_RELEASE

# # Add nxp yocto sources
# #RUN python3 /opt/repo/repo init -u https://github.com/SolidRun/meta-solidrun-arm-imx8 -b kirkstone-imx8m -m sr-imx-5.15.71-2.2.0.xml
# #RUN python3 /opt/repo/repo sync

# # Add meta-raspberry layer 
# RUN git clone https://github.com/agherzan/meta-raspberrypi  -b $YOCTO_RELEASE
# #RUN cd meta-raspberrypi && git checkout 59a6a1b5dd1e21189adec49c61eae04ed3e70338  && cd ..

# # Add meta-qt5 layer
# RUN git clone https://github.com/meta-qt5/meta-qt5/tree/kirkstone -b $YOCTO_RELEASE

# # Add realsense repo
# #RUN git clone https://github.com/IntelRealSense/meta-intel-realsense.git -b $YOCTO_RELEASE

# # Add linux-imx kernel patches -- https://github.com/IntelRealSense/meta-intel-realsense/issues/25
# # WORKDIR /poky/meta-intel-realsense

# # The last commit with linux-imx patches  is ff4f997921f0be10eb5e5d5777b4a0e911cfaf8d
# #RUN git checkout ff4f997921f0be10eb5e5d5777b4a0e911cfaf8d

# # To get the new librealsense try to checkout linux-imx patches only....
# #RUN git checkout ff4f997921f0be10eb5e5d5777b4a0e911cfaf8d -- recipes-kernel/linux/linux-imx_5.4.%.bbappend
# #RUN git checkout ff4f997921f0be10eb5e5d5777b4a0e911cfaf8d -- recipes-kernel/linux/realsense-linux-5.4.inc
# #RUN git checkout ff4f997921f0be10eb5e5d5777b4a0e911cfaf8d -- recipes-kernel/linux/files

# # Aplly all patches
# COPY .yocto/patches/ /tmp/patches
# #RUN cd /poky/sources/meta-openembedded  && git am /tmp/patches/meta-openembedded/*/*.patch
# RUN cd /poky/meta-intel-realsense       && git am /tmp/patches/realsense/*.patch

# WORKDIR /poky

# # Download sources for each and every software piece used in the image
# RUN mkdir /poky/build
# COPY .yocto/conf /poky/build/conf
# COPY .yocto/layers /layers
# COPY . /src
# RUN bash -c "export MACHINE=raspberrypi4-64 && source /poky/sources/poky/oe-init-build-env &&  bitbake image-release --runall fetch"
# RUN sudo rm -rf /poky/build/conf /layers /sls 


# # Yocto dir structure setup
# RUN mkdir -p /poky/build
# # As the entre /poky/build directory is mounted from docker volume, the 
# # /poky/build/conf can`t be configured during docker build; This directory
# # is created by setup.sh(bat) script.
# RUN sudo ln -sf  poky/build/conf         /poky/build/conf
# RUN sudo ln -sf /src/.yocto/layers       /layers
# RUN sudo ln -sf /src/.yocto/build/bin    /binaries
# RUN sudo ln -sf /src/.yocto/util         /util

# RUN echo "cd /poky && source /poky/sources/poky/oe-init-build-env" >> /home/pokyuser/.bashrc

# ENTRYPOINT [ "/bin/bash", "--rcfile", "/home/pokyuser/.bashrc", "-i"]
