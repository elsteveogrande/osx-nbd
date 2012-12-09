#!/bin/bash

HOST=osxtest
scp -r ~/Library/Developer/Xcode/DerivedData/NBDDisk-*/Build/Products/Debug/NBDDisk.kext "$HOST":~

