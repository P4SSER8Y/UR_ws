#!/bin/bash
# author: Qining Zhang
# date: 2018-04-19
# TEST ON Ubuntu 16.04.4 LTS & Ubuntu 14.04.5 LTS

pushd

sudo apt-get update -qq
sudo apt-get install -y libudev-dev libusb-1.0-0-dev libfox-1.6-dev git
sudo apt-get install autotools-dev autoconf automake libtool -y

cd /tmp
git clone git://github.com/signal11/hidapi.git
cd hidapi
./bootstrap
./configure
make
sudo make install

cd /tmp
git clone https://github.com/wjasper/Linux_Drivers.git
cd Linux_Drivers
sudo cp 61-mcc.rules /etc/udev/rules.d
sudo /sbin/udevadm control --reload
cd USB/mcc-libusb
make
sudo make install
sudo ldconfig

popd

