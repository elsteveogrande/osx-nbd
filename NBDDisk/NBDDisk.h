#include <IOKit/IOService.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

#include "NBDDiskStorageDevice.h"


class cc_obrien_NBDDisk : public IOService
{
	OSDeclareDefaultStructors(cc_obrien_NBDDisk)
	
private:
	IOBufferMemoryDescriptor *descriptor;
	void *buffer;
	bool createBlockStorageDevice();

public:
	virtual bool start(IOService *provider);
	virtual void free();
	virtual IOByteCount performRead(IOMemoryDescriptor *desc, UInt64 offset, UInt64 length);
	virtual IOByteCount performWrite(IOMemoryDescriptor *desc, UInt64 offset, UInt64 length);
};
