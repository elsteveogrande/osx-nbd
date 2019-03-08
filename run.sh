#!/bin/bash

set -e
set -x

KEXT=build/Release/NBDDisk.kext

sudo chown -R root:wheel "${KEXT}"
sudo chmod 755 "${KEXT}"
sudo kextutil -v "${KEXT}"
