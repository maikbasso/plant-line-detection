# KEEP UBUNTU OR DEBIAN UP TO DATE

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y dist-upgrade
sudo apt-get -y autoremove


# INSTALL THE DEPENDENCIES

sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

# INSTALL THE LIBRARY 

cd ~

git clone https://github.com/Itseez/opencv.git

cd ~/opencv

mkdir build

cd build

cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..

make -j7

sudo make install

sudo ldconfig