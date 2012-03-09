GCC=gcc
KCC=$(GCC) -framework Kernel
KEXTDIR=/tmp/osx-nbd/NBD.kext

all: kext

kext:
	$(KCC) -O0 -o nbd  *.cpp

test: kext
	rm -rf $(KEXTDIR)
	mkdir -p $(KEXTDIR)
	cp nbd $(KEXTDIR)/Contents
	cp Info.plist $(KEXTDIR)
	chown root:wheel $(KEXTDIR)
	chown 0755 $(KEXTDIR)
	kextload $(KEXTDIR)

stoptest:
	kextunload $(KEXTDIR)
	rm -rf $(KEXTDIR)
