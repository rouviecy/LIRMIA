# LIRMIA project

### Dependencies
+    **CMake**
+    **GCC** for c++11
+    **GNU/Linux**
+    **OpenCV** (minimum 2.4) with Gtk
+    **V4L** driver (for Raspberry Pi camera)
+    **SDL2** (for keyboard and joystick of remote controller)
+    **DOT** (optional : to generate graphs)

### Enable C++11 (for Ubuntu >= 14.04)

    sudo apt-get install build-essential g++-4.8 cmake
    sudo rm /usr/bin/gcc
    sudo rm /usr/bin/g++
    sudo ln -s /usr/bin/gcc-4.8 /usr/bin/gcc
    sudo ln -s /usr/bin/g++-4.8 /usr/bin/g++

### Raspberry Pi
Enable camera (load `bcm2835-v4l2` and `v4l2-ctl` after each boot) :

    sudo raspi-config
    sudo apt-get install v4l-utils
    sudo modprobe bcm2835-v4l2
    sudo v4l2-ctl --set-fmt-video=width=200,height=150,pixelformat="H264" -d /dev/video0

Disable Serial over console :

    sudo raspi-config

Enable I2C (load `i2c-dev` after each boot) :

    sudo raspi-config
    sudo apt-get install i2c-tools
    sudo modprobe i2c-dev

Enable SPI (load `spi_bcm2708` and `spidev` after each boot) :

    sudo raspi-config
    sudo modprobe spi_bcm2708
    sudo modprobe spidev

### Compile
Remember to adjust `IS_XXX` in `CMakeLists.txt` to choose a robot or a simulator/remote.

    git clone https://github.com/rouviecy/LIRMIA
    cd LIRMIA/build
    cmake ..
    make

### Execute
On LIRMIA robot :

    sudo ./lirmia

On R2D2 robot :

    sudo ./r2d2

On Cookie robot :

    sudo ./cookie

On Sayab robot :

    sudo ./sayab

On remote controller :

    ./camera_XXX <robot IP>
    ./remote_XXX <robot IP>
