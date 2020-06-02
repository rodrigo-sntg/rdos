# RDOS - Rodrigo Operational System

First try developing an operational system, using C and Assembly.


Until Now, all we can do on this OS, is run on grub and type something.

## Features till now:
```
loader
linker
kernel compiling
interrupts handling
keyboard handling
```

To start start from here, you'll need the tool kit for OS development, run:
```
sudo apt-get install build-essential nasm genisoimage bochs bochs-sdl
```

The system is running on VirtualBox.
So to start it, you should run:
```
make run
```
and to erase not essential files:
```
make clean
```







References:

[Little Book of OS](https://littleosbook.github.io/)    
[OS DEV](https://wiki.osdev.org/Beginner_Mistakes)  
[Little Book Of OS  GITHUB](https://littleosbook.github.io/#virtual-machine)
