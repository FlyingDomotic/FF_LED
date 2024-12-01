# Implement few LED effects (fixed, blinking, pulsing) with brightness management

This class implements few effects on a LED, with brightness management.

## Prerequisites/ Prérequis

Can be used directly with Arduino IDE or PlatformIO.

## Installation

Clone repository somewhere on your disk.
```
cd <where_you_want_to_install_it>
git clone https://github.com/FlyingDomotic/FF_LED.git FF_LED
```

Note that <where_you_want_to_install_it> could ingeniously be your compiler libraries folder ;-)

## Update

Go to code folder and pull new version:
```
cd <where_you_installed_FF_LED>
git pull
```

Note: if you did any changes to files and `git pull` command doesn't work for you anymore, you could stash all local changes using:
```
git stash
```
or
```
git checkout <modified file>
```

## Documentation

Documentation could be built using doxygen, either using makedoc.sh (for Linux) or makedoc.bat (for Windows), or running
```
doxygen doxyfile
```

HTML and RTF versions will then be available in `documentation` folder.
