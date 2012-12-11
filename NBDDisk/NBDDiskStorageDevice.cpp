//
//  NBDDiskStorageDevice.cpp
//  NBDDisk
//
//  Created by Steve O'Brien on 11/13/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#include <IOKit/storage/IOBlockStorageDevice.h>
#include "NBDDiskStorageDevice.h"

#define super IOBlockStorageDevice


OSDefineMetaClassAndStructors(cc_obrien_NBDDiskStorageDevice, super)


bool cc_obrien_NBDDiskStorageDevice::init(OSDictionary *properties)
{
	if(! super::init(properties))
	{
		return false;
	}
	
	if(provider->getByteCount() % provider->getBlockSize())
	{
		// not divisible?!
		return false;
	}
	
	this->blockCount = provider->getByteCount() / provider->getBlockSize();

	return true;
}


bool cc_obrien_NBDDiskStorageDevice::attach(IOService *provider)
{
	if(! super::attach(provider))
	{
		return false;
	}
	
	this->provider = OSDynamicCast(cc_obrien_NBDBlockService, provider);
	if(! this->provider)
	{
		return false;
	}
	
	return true;
}


void cc_obrien_NBDDiskStorageDevice::detach(IOService *provider)
{
	if(provider == this->provider)
	{
		this->provider = NULL;
	}
	
	super::detach(provider);
}


IOReturn cc_obrien_NBDDiskStorageDevice::doEjectMedia()
{
	return kIOReturnUnsupported;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doFormatMedia(UInt64 byteCapacity)
{
	return kIOReturnUnsupported;
}


UInt32 cc_obrien_NBDDiskStorageDevice::doGetFormatCapacities(UInt64 *byteCapacities, UInt32 capacitiesMaxCount) const
{
	if(!byteCapacities || capacitiesMaxCount < 1)
	{
		return 0;
	}
	
	byteCapacities[0] = this->provider->getByteCount();
	return 1;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doLockUnlockMedia(bool doLock)
{
	if(doLock)
		return kIOReturnUnsupported;
	
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doSynchronizeCache()
{
	// TODO
	return kIOReturnSuccess;
}


char * cc_obrien_NBDDiskStorageDevice::getVendorString()
{
	return (char *) "(networked)";
}


char * cc_obrien_NBDDiskStorageDevice::getProductString()
{
	return (char *) "NBD Disk";
}


char * cc_obrien_NBDDiskStorageDevice::getRevisionString()
{
	return (char *) "1";
}


char * cc_obrien_NBDDiskStorageDevice::getAdditionalDeviceInfoString()
{
	return (char *) "nbd device host=%s port=%d size=%lld bytes";
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportBlockSize(UInt64 *blockSize)
{
	*blockSize = (UInt64) this->provider->getBlockSize();
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportEjectability(bool *isEjectable)
{
	*isEjectable = false;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportLockability(bool *isLockable)
{
	*isLockable = false;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportMaxValidBlock(UInt64 *maxBlock)
{
	*maxBlock = this->blockCount - 1;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportMediaState(bool *mediaPresent, bool *changedState)
{
	*mediaPresent = (this->provider && this->provider->isReady());
	*changedState = false;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportPollRequirements(bool *pollRequired, bool *pollIsExpensive)
{
	*pollRequired = true;
	*pollIsExpensive = false;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportRemovability(bool *isRemovable)
{
	*isRemovable = true;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportWriteProtection(bool *isWriteProtected)
{
	*isWriteProtected = ! (this->provider && this->provider->isWritable());
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::getWriteCacheState(bool *enabled)
{
	*enabled = false;
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::setWriteCacheState(bool enabled)
{
	return kIOReturnUnsupported;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doAsyncReadWrite(IOMemoryDescriptor *buffer, UInt64 block, UInt64 nblks, IOStorageAttributes *attributes, IOStorageCompletion *completion)
{
	IOByteCount actualCount = 0;
	
	if(!this->provider || !this->provider->isReady())
	{
		return kIOReturnNotAttached;
	}
	
	if(block + nblks > this->blockCount)
	{
		return kIOReturnBadArgument;
	}

	if(buffer->getDirection() == kIODirectionIn)
	{
		actualCount = provider->performRead(
			buffer,
			block * this->provider->getBlockSize(),
			nblks * this->provider->getBlockSize());
	}
	else if(buffer->getDirection() == kIODirectionIn)
	{
		if(! this->provider->isWritable())
		{
			return kIOReturnNotWritable;
		}

		actualCount = provider->performWrite(
			buffer,
			block * this->provider->getBlockSize(),
			nblks * this->provider->getBlockSize());
	}
	else
	{
		return kIOReturnBadArgument;
	}
	
	(completion->action)(completion->target, completion->parameter, kIOReturnSuccess, actualCount);
	return kIOReturnSuccess;
}
