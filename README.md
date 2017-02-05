# [AMixST](https://github.com/FenixFyreX/amixst)
--------------
## Overview
AMixST (AlsaMixer Sound Tool) is an open source and free volume wheel / mixer written with Qt5 and alsa released under GPLv3 as per QT regulations.

## Requirements
#### Runtime
- qt5-base
- alsa-lib

#### Optional (for themes, etc)
- qt5-styleplugins

#### Build
- qt5-tools
- gcc (g++)

Note: Package names my be different per distro. This app was written using Arch Linux.

## Compilation
amixst was written for systems running alsa. Usually this means some form of linux. No testing was done for Mac or Windows, and no guarantees on compilation status for these latter platforms is made.

There is an Arch Linux PKGBUILD distributed with these files.
Consider using it:
```
mkdir amixst && curl https://raw.githubusercontent.com/FenixFyreX/amixst/master/archlinux/PKGBUILD > amixst/PKGBUILD
cd amixst && makepkg
pacman -S amixst-pkg.tar.gz
```

Generic Linux process:
```
git clone https://github.com/FenixFyreX/amixst.git
cd amixst
mkdir amixst/build && cd amixst/build
qt5-qmake ../src/amixst.pro CONFIG+=release -spec=linux-g++
make -j `grep -c ^processor /proc/cpuinfo`
sudo install -m755 ./amixst /usr/bin
mkdir ~/.config/amixst
cp ../etc/conf ~/.config/amixst/conf
sudo install -dm644 /usr/share/amixst
sudo install -m755 ../LICENSE /usr/share/amixst
sudo install -m755 ../COPYING /usr/share/amixst
```

## Usage
It is recommended to start this program after your given window manager starts, and most likely after the system tray is 'visible'. For instance, running openbox, this app can be started from ~/.config/openbox/autostart.

It is recommended that some form of system tray visualizer is in place so that the tray icon (and thus the entirety of this app) is accessible.

## Config
Along with the program comes a config file which will be located at: `/etc/amixst/conf`
This file can be copied to `~/.config/amixst/conf` and edited. The config file will tell you what it can do.

## Bug Reports / Feature Requests
If an issue is found or you have a feature suggestion, the defacto [github issue tracker](https://github.com/FenixFyreX/amixst/issues) can be used.

## Planned for Future Versions
- Actual mixer dialog that allows selection of card/device and provides controls for every channel of said card/device.
