
#include <sys/types.h>
#include "NBDDisk.h"
#include "NBDDiskStorageDevice.h"

#define super IOService

#define DISK_SIZE 16777216

OSDefineMetaClassAndStructors(cc_obrien_NBDDisk, IOService)


bool cc_obrien_NBDDisk::start(IOService *provider)
{
	if(! super::start(provider))
	{
		return false;
	}
	
	this->descriptor = IOBufferMemoryDescriptor::withCapacity(DISK_SIZE, kIODirectionOutIn);
	if(! this->descriptor)
	{
		return false;
	}
	
	this->buffer = descriptor->getBytesNoCopy();
	
	if(! this->createBlockStorageDevice())
	{
		return false;
	}
	
	return true;
}


bool cc_obrien_NBDDisk::createBlockStorageDevice()
{
	cc_obrien_NBDDiskStorageDevice *nub = 0;
	bool result = false;
	
	nub = new cc_obrien_NBDDiskStorageDevice();
	if(! nub)
	{
		return false;
	}
	
	if(! nub->init(DISK_SIZE))
	{
		goto out;
	}
	
	if(! nub->attach(this))
	{
		goto out;
	}
	
	nub->registerService();
	result = true;
	
out:
	if(nub)  nub->release();
	return result;
}


void cc_obrien_NBDDisk::free()
{
	if(descriptor)
	{
		descriptor->release();
	}
	
	super::free();
}


IOByteCount cc_obrien_NBDDisk::performRead(IOMemoryDescriptor *desc, unsigned long long offset, unsigned long long length)
{
	return desc->readBytes(0, (void *) (offset + (uintptr_t) buffer), length);
}


IOByteCount cc_obrien_NBDDisk::performWrite(IOMemoryDescriptor *desc, unsigned long long offset, unsigned long long length)
{
	return desc->writeBytes(0, (void *) (offset + (uintptr_t) buffer), length);
}


