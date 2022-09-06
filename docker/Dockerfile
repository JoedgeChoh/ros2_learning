FROM osrf/ros:galactic-desktop
MAINTAINER  "zhuhe<zhuhe@tongxin.cn>"

RUN sudo apt-get update
RUN echo "source /opt/ros/galactic/local_setup.bash" > /etc/bash.bashrc
RUN sudo apt-get install vim -y
RUN apt-get update &&\
	apt-get -y install sudo

# 添加docker用户，并重置密码,增加sudo权限
# RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
# USER docker

# COPY entrypoint.sh /entrypoint.sh
# ENTRYPOINT ["bash","/entrypoint.sh"] 
