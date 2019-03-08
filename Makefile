all: nbd-kext

nbd-kext: build/Release/NBDDisk.kext

build/Release/NBDDisk.kext: NBDDisk/*.cc NBDDisk/*.h
	xcodebuild -quiet

load:
	sudo rm -rf /tmp/nbddisk-kext
	sudo cp -rp build/Release/NBDDisk.kext /tmp/nbddisk-kext
	sudo chown -R root:wheel /tmp/nbddisk-kext/NBDDisk.kext
	sudo chmod 755 /tmp/nbddisk-kext/NBDDisk.kext
	sudo kextload -v /tmp/nbddisk-kext/NBDDisk.kext

unload:
	sudo kextunload -v /tmp/nbddisk-kext/NBDDisk.kext

clean:
	rm -rf build
