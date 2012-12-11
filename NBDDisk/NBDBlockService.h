//
//  NBDIOStorage.h
//  NBDDisk
//
//  Created by Steve O'Brien on 12/8/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#ifndef __NBDDisk__NBDBlockService__
#define __NBDDisk__NBDBlockService__

#include <IOKit/IOService.h>
#include <IOKit/IOBufferMemoryDescriptor.h>


class cc_obrien_NBDBlockService : public IOService
{
	OSDeclareDefaultStructors(cc_obrien_NBDBlockService)

private:
	IOBufferMemoryDescriptor *memory;
	void *buffer;
	
protected:
	bool buildDevice();

public:
	virtual bool start(IOService *provider);
	virtual void free();
	virtual IOByteCount performRead(IOMemoryDescriptor *dest, UInt64 offset, UInt64 count);
	virtual IOByteCount performWrite(IOMemoryDescriptor *src, UInt64 offset, UInt64 count);
	
	UInt64 getByteCount() const;
	UInt32 getBlockSize() const;
	bool isWritable();
	bool isReady();
};


#endif /* defined(__NBDDisk__NBDBlockService__) */
