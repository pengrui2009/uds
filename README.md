# uds
uds protocol will stepply satisfied iso14229-1-2-3 and iso15765-2

## support can device

kvaser

## install kvaser driver
### install C/C++ lib
sudo apt-get remove -y linuxcan-dkms;sudo rm -rf /usr/src/linuxcan*
sudo rm -rf /usr/local/sbin/listChannels
sudo apt-add-repository -y ppa:jwhitleyastuff/linuxcan-dkms
sudo apt-get update;sudo apt-get install -y linuxcan-dkms
#### install python interface
pip install canlib-1.8.812-py2.py3-none-any.whl --user

lsmod| grep -q kvpcicanII
if [[ $? -eq 1 ]]; then
    echo "kvaser ko modules is not loaded, load them automaticly!!!"
    if [[ ! -f "/etc/modules-load.d/kvaser.conf" ]]; then
        cp kvaser.conf /etc/modules-load.d/
    fi
    systemctl restart systemd-modules-load.service
fi

rm -f /usr/local/sbin/listChannels
sudo ln -s /usr/src/linuxcan*/canlib/examples/listChannels /usr/local/sbin/

## the method of compile in Linux system.
mkdir build && cd build
cmake ..
make