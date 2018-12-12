# code-war-badge

A Particle project named code-war-badge

## Compiling for mesh devices:

In theory, this code should compile for all mesh devices, hopefully the code will one day split functionality up
reasonablly between the gateway and non-gateway devices.

Compiling locally involves the following:

*Note: The `APPDIR` Directive wants a full path! `~` will not work as expected!*

```
# Get this repo:
cd ~/dev
git clone

# Get working versions of the libraries and symbolically link them into the project
# (Particle is working on updating these to work with Mesh devices. Symlinks will go away 
#  at that point):

## Neopixel:
cd ~/dev
git clone git@github.com:technobly/Particle-NeoPixel.git
cd ~/dev/Particle-NeoPixel/
git pull origin pull/38/head
ln -s ~/dev/Particle-NeoPixel ~/dev/code-war-badge/lib/neopixel

## Adafruit LED Backpack:
cd ~/dev
git clone git@github.com:creationix/spark-adafruit-led-backpack.git
ln -s ~/dev/spark-adafruit-led-backpack ~/dev/code-war/badge/lib/adafruit-led-backpack

# Get the Particle firmware repo:
cd ~/dev
git clone git@github.com:particle-iot/firmware.git
cd ~/dev/firmware

# Until mesh-develop is merged back to mainline branch:
git checkout mesh-develop

git submodule update --init

# First build
cd ~/dev/firmware/modules/
make clean all PLATFORM=xenon APPDIR=$HOME/dev/code-war-badge

# Ongoing builds:
cd ~/dev/firmware/main/
make all PLATFORM=xenon APPDIR=$HOME/dev/code-war-badge

particle flash <device> ~/dev/code-war-badge/target/code-war-badge.bin
```
