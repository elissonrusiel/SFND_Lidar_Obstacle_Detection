#!/bin/bash

sudo apt -y update
sudo apt -y install git build-essential linux-libc-dev
sudo apt -y install cmake cmake-gui
sudo apt -y install libusb-dev libudev-dev
sudo apt -y install mpi-default-dev openmpi-bin openmpi-common

sudo apt -y install libpcap-dev
sudo apt -y install libflann1.8 libflann-dev
sudo apt -y install libeigen3-dev
sudo apt -y install libopenni2-dev
sudo apt -y install libqhull7 libqhull-dev

sudo apt -y install freeglut3-dev pkg-config
sudo apt -y install libxmu-dev libxi-dev
sudo apt -y install mono-complete
sudo apt -y install openjdk-8-jdk openjdk-8-jre

sudo apt -y install mesa-utils

# Fix OPENGL error: 

glxinfo | grep OpenGL

sudo adduser $USER video
ubuntu-drivers devices
sudo ubuntu-drivers autoinstall
sudo apt autoremove

glxgears

