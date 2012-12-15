#!/bin/bash

HOST=osxtest
scp -r ~/Library/Developer/Xcode/DerivedData/NBDDisk-*/Build/Products/Debug/NBDDisk.kext "$HOST":~
gcc -m32 -m64 -O0 -o nbd-setup nbd-setup.c
scp nbd-setup "$HOST":~

