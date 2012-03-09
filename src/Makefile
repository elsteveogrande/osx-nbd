CC_ARCH=-m32 -m64
LD_ARCH=-arch i386 -arch x86_64
SDK=/Developer/SDKs/MacOSX10.7.sdk
GCC=gcc -g -O0 $(CC_ARCH) -Wall -fmessage-length=0 -nostdinc -fno-builtin -Wno-trigraphs -fno-exceptions -force_cpusubtype_ALL -msoft-float -O0 -fno-common -mkernel -finline -fno-keep-inline-functions -Wmissing-prototypes -Wreturn-type -Wunused-variable -Wshorten-64-to-32 -DDEBUG=1 -DKERNEL -DKERNEL_PRIVATE -DDRIVER_PRIVATE -DAPPLE -DNeXT -isysroot $(SDK) -fasm-blocks -mmacosx-version-min=10.5 -gdwarf-2 -I/System/Library/Frameworks/Kernel.framework/PrivateHeaders -I$(SDK)/System/Library/Frameworks/Kernel.framework/Headers -I.
GCCPP=$(GCC) -x c++ -fno-rtti -fcheck-new -fapple-kext
LD=ld $(LD_ARCH) -lcpp_kext -kext -lkmodc++ -lkmod
KEXTDIR=/tmp/osx-nbd/NBD.kext

all: kext

kext: nbd.o device.o
	$(LD) -o nbd nbd.o device.o

nbd.o: nbd.cpp
	$(GCCPP) -nostdinc -nostdlib -O0 -o nbd.o -c nbd.cpp

device.o: device.cpp
	$(GCCPP) -nostdinc -nostdlib -O0 -o device.o -c device.cpp

test: kext
	rm -rf $(KEXTDIR)
	mkdir -p $(KEXTDIR)/Contents/MacOS
	cp Info.plist $(KEXTDIR)/Contents
	cp nbd $(KEXTDIR)/Contents/MacOS
	chown root:wheel $(KEXTDIR)
	chmod 755 $(KEXTDIR)
	kextload $(KEXTDIR)

stoptest:
	kextunload $(KEXTDIR)
	rm -rf $(KEXTDIR)

clean:
	rm -rf $(KEXTDIR)
	rm -f nbd *.o
