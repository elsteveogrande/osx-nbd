#!/bin/bash

# (to install into kernel)
sudo chown -R root:wheel NBDDisk.kext && sudo chmod 755 NBDDisk.kext && sudo kextutil -v NBDDisk.kext

