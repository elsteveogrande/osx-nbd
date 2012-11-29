//
//  NBDDiskStorageDevice.h
//  NBDDisk
//
//  Created by Steve O'Brien on 11/13/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef __NBDDisk__NBDDiskStorageDevice__
#define __NBDDisk__NBDDiskStorageDevice__

#include <IOKit/storage/IOBlockStorageDevice.h>


class cc_obrien_NBDDiskStorageDevice : public IOBlockStorageDevice
{
	OSDeclareDefaultStructors(cc_obrien_NBDDiskStorageDevice)
	
private:
	void wipe(unsigned long long block, unsigned long long count);

protected:

public:
	virtual bool init(UInt64 diskSize, OSDictionary *properties = 0);
	virtual IOReturn doEjectMedia();
	virtual IOReturn doFormatMedia(UInt64 byteCapacity);
	virtual UInt32 doGetFormatCapacities(UInt64 *byteCapacity, UInt32 capacitiesMaxCount) const;
	virtual IOReturn doLockUnlockMedia(bool doLock);
	virtual IOReturn doSynchronizeCache();
	virtual char * getVendorString();
	virtual char * getProductString();
	virtual char * getRevisionString();
	virtual char * getAdditionalDeviceInfoString();
	virtual IOReturn reportBlockSize(UInt64 *blockSize);
	virtual IOReturn reportEjectability(bool *isEjectable);
	virtual IOReturn reportLockability(bool *isLockable);
	virtual IOReturn reportMaxValidBlock(UInt64 *maxBlock);
	virtual IOReturn reportMediaState(bool *mediaPresent, bool *changedState);
	virtual IOReturn reportPollRequirements(bool *pollRequired, bool *pollIsExpensive);
	virtual IOReturn reportRemovability(bool *isRemovable);
	virtual IOReturn reportWriteProtection(bool *isWriteProtected);
	virtual IOReturn getWriteCacheState(bool *enabled);
	virtual IOReturn setWriteCacheState(bool enabled);
	virtual IOReturn doAsyncReadWrite(IOMemoryDescriptor *buffer, UInt64 block, UInt64 nblks, IOStorageAttributes *attributes, IOStorageCompletion *completion);
	virtual IOReturn requestIdle();
	virtual IOReturn doDiscard(UInt64 block, UInt64 nblks);
	virtual IOReturn doUnmap(IOBlockStorageDeviceExtent *extents, UInt32 extentsCount, UInt32 options);
};

#endif /* defined(__NBDDisk__NBDDiskStorageDevice__) */
