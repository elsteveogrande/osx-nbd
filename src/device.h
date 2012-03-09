/*
 * (C) 2012 Steve O'Brien.  BSD licensed.
 * see http://www.opensource.org/licenses/BSD-2-Clause
 * and see LICENSE in the root of this project.
 */


#include <libkern/libkern.h>
#include <mach/kmod.h>
#include <IOKit/storage/IOBlockStorageDevice.h>

class NBDBlockStorageDevice : public IOBlockStorageDevice
{
public:
	virtual IOReturn doEjectMedia();
	virtual IOReturn doFormatMedia(UInt64);
	virtual UInt32 doGetFormatCapacities(UInt64*, UInt32) const;
	virtual IOReturn doSynchronizeCache();
	virtual char* getVendorString();
	virtual char* getProductString();
	virtual char* getRevisionString();
	virtual char* getAdditionalDeviceInfoString();
	virtual IOReturn reportBlockSize(UInt64*);
	virtual IOReturn reportEjectability(bool*);
	virtual IOReturn reportMaxValidBlock(UInt64*);
	virtual IOReturn reportMediaState(bool*, bool*);
	virtual IOReturn reportRemovability(bool*);
	virtual IOReturn reportWriteProtection(bool*);
	virtual IOReturn getWriteCacheState(bool*);
	virtual IOReturn setWriteCacheState(bool);
	virtual IOReturn doAsyncReadWrite(IOMemoryDescriptor*, UInt64, UInt64, IOStorageAttributes*, IOStorageCompletion*);

	void stop();
};
