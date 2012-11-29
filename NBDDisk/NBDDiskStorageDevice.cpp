//
//  NBDDiskStorageDevice.cpp
//  NBDDisk
//
//  Created by Steve O'Brien on 11/13/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#include "NBDDiskStorageDevice.h"


bool cc_obrien_NBDDiskStorageDevice::IOBlockStorageDevice::init(OSDictionary *properties)
{
	return true;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doEjectMedia()
{
	return kIOReturnUnsupported;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doFormatMedia(UInt64 byteCapacity)
{
	return kIOReturnUnsupported;
}


UInt32 cc_obrien_NBDDiskStorageDevice::doGetFormatCapacities(UInt64 *byteCapacity, UInt32 capacitiesMaxCount) const
{
	if(byteCapacity != 0 && capacitiesMaxCount >= 1)
	{
		*byteCapacity = size();
	}
	
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
	return (char *) "obrien.cc";
}


char * cc_obrien_NBDDiskStorageDevice::getProductString()
{
	return (char *) "LBD";
}


char * cc_obrien_NBDDiskStorageDevice::getRevisionString()
{
	return (char *) "1";
}


char * cc_obrien_NBDDiskStorageDevice::getAdditionalDeviceInfoString()
{
	return (char *) "lbd device host=%s port=%d size=%lld bytes";
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportBlockSize(UInt64 *blockSize)
{
	*blockSize = BS();
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
	UInt64 blockSize;
	this->reportBlockSize(&blockSize);
	return size() / blockSize;
}


IOReturn cc_obrien_NBDDiskStorageDevice::reportMediaState(bool *mediaPresent, bool *changedState)
{
	mediastate();
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
	*isWriteProtected = ! writable();
}


IOReturn cc_obrien_NBDDiskStorageDevice::getWriteCacheState(bool *enabled)
{
	*enabled = false;
}


IOReturn cc_obrien_NBDDiskStorageDevice::setWriteCacheState(bool enabled)
{
	return kIOReturnUnsupported;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doAsyncReadWrite(IOMemoryDescriptor *buffer, UInt64 block, UInt64 nblks, IOStorageAttributes *attributes, IOStorageCompletion *completion)
{
	io();
}


IOReturn cc_obrien_NBDDiskStorageDevice::requestIdle()
{
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doDiscard(UInt64 block, UInt64 nblks)
{
	this->wipe(block, nblks);
	return kIOReturnSuccess;
}


IOReturn cc_obrien_NBDDiskStorageDevice::doUnmap(IOBlockStorageDeviceExtent *extents, UInt32 extentsCount, UInt32 options)
{
	int i;
	
	for(i=0; i<extentsCount; i++)
	{
		// TODO
	}

	return kIOReturnSuccess;
}


void cc_obrien_NBDDiskStorageDevice::wipe(unsigned long long block, unsigned long long count)
{
	// TODO
}

