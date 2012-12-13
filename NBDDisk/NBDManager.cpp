//
//  NBDManager.cpp
//  NBDDisk
//
//  Created by Steve O'Brien on 12/11/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#include <sys/types.h>                       // (miscfs/devfs/devfs.h, ...)
#include <miscfs/devfs/devfs.h>              // (devfs_make_node, ...)
#include <sys/buf.h>                         // (buf_t, ...)
#include <sys/fcntl.h>                       // (FWRITE, ...)
#include <sys/ioccom.h>                      // (IOCGROUP, ...)
#include <sys/proc.h>                        // (proc_is64bit, ...)
#include <sys/stat.h>                        // (S_ISBLK, ...)
#include <sys/systm.h>                       // (DEV_BSIZE, ...)
#include <IOKit/assert.h>
#include <IOKit/IOBSD.h>
#include <IOKit/IODeviceTreeSupport.h>
#include <IOKit/IOKitKeys.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOSubMemoryDescriptor.h>
#include <IOKit/storage/IOBlockStorageDevice.h>
#include <IOKit/storage/IOBlockStorageDriver.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOMediaBSDClient.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include "NBDManager.h"
#include "NBDBlockService.h"

#define super IOService

OSDefineMetaClassAndStructors(cc_obrien_NBDManager, super)


extern "C"
{
	struct cdevsw table_cdevsw =
	{
		/* d_open     */ nbd_open,
		/* d_close    */ nbd_close,
		/* d_read     */ nbd_read,
		/* d_write    */ nbd_write,
		/* d_ioctl    */ nbd_ioctl,
		/* d_stop     */ nbd_stop,
		/* d_reset    */ nbd_reset,
		/* d_ttys     */ 0,
		/* d_select   */ nbd_select,
		/* d_mmap     */ eno_mmap,
		/* d_strategy */ eno_strat,
		/* d_getc     */ eno_getc,
		/* d_putc     */ eno_putc,
		/* d_type     */ D_TTY,
	};
	
	void *instance;  /* instance of NBDManager */
}


bool cc_obrien_NBDManager::init(OSDictionary *properties)
{
	this->dev = NULL;
	this->major = -1;

	IOLog("nbd: initing --------------------------------------\n");
	if(! super::init(properties))
	{
		return false;
	}

	instance = this;
	IOLog("nbd: inited %p\n", instance);
	return true;
}


void cc_obrien_NBDManager::free()
{
	IOLog("nbd: free()\n");
	super::free();
}


bool cc_obrien_NBDManager::start(IOService *provider)
{
	bool ret = false;
	
	IOLog("nbd: starting\n");
	if(! (super::start(provider)))
	{
		return false;
	}

	this->major = cdevsw_add(-1, &table_cdevsw);
	IOLog("nbd init: got major %d\n", this->major);
	if(this->major < 0)
	{
		ret = false;
		goto out;
	}

	this->dev = devfs_make_node(makedev(this->major, 0), DEVFS_CHAR, UID_ROOT, GID_WHEEL, 0400, "nbdc");
	if(! this->dev)
	{
		goto abort_cdev_major;
	}

	IOLog("nbd: start SUCCESS\n");
	ret = true;
	goto out;

abort_cdev_major:
	IOLog("nbd: start aborting!\n");
	cdevsw_remove(this->major, &table_cdevsw);
	this->major = -1;

out:
	IOLog("nbd: start func complete\n");
	return ret;
}


void cc_obrien_NBDManager::stop(IOService *provider)
{
	if(this->dev)
	{
		devfs_remove(this->dev);
		this->dev = NULL;
	}

	if(this->major != -1)
	{
		cdevsw_remove(this->major, &table_cdevsw);
	}

	IOLog("nbd: stop()\n");
	super::stop(provider);
}


IOService * cc_obrien_NBDManager::probe(IOService *provider, SInt32 *score)
{
	IOLog("nbd: probe (super)\n");
	return super::probe(provider, score);
}


// callable from C

int nbd_ioctl(dev_t dev, u_long cmd, caddr_t data, int flags, proc_t proc)
{
	IOLog("nbd ioctl: dev=0x%x cmd=0x%lx data=%p flags=0x%x proc=%p\n", dev, cmd, data, flags, proc);
	return EIO;
}

int nbd_open(dev_t dev, int flags, int devtype, proc_t proc)
{
	IOLog("nbd open: *** dev=0x%x flags=%x devtype=%x proc=%p\n", dev, flags, devtype, proc);
	
	cc_obrien_NBDBlockService *bs = new cc_obrien_NBDBlockService;
	bs->init(NULL);  // null properties dictionary
	bs->attachToParent(IORegistryEntry::getRegistryRoot(), gIOServicePlane);
	bs->start(NULL);
	bs->registerService();

	return 0;
}

int nbd_close(dev_t dev, int flags, int devtype, proc_t proc)
{
	return 0;
}

int nbd_read(dev_t dev, uio_t uio, int flags)
{
	return EIO;
}

int nbd_write(dev_t dev, uio_t uio, int flags)
{
	return EIO;
}

int nbd_reset(int x1)
{
	return EIO;
}

int nbd_select(dev_t x1, int x2, void *x3, proc_t x4)
{
	return EIO;
}

int nbd_stop(struct tty *x1, int x2)
{
	return EIO;
}
