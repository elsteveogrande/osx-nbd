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
#include <sys/kpi_socket.h>

#include "NBDDiskStorageDevice.h"


class cc_obrien_NBDBlockService : public IOService
{
	OSDeclareDefaultStructors(cc_obrien_NBDBlockService)

private:
	IOBufferMemoryDescriptor *memory;
	void *buffer;
	socket_t *socket;   /* 0 when uninitialized / not yet connected / error occurred and we disconnected */
	UInt64 size;
	cc_obrien_NBDDiskStorageDevice *nub;
	
protected:
	bool buildDevice();
	void shutdown();

public:
	void setSizeMB(int mb);
	virtual bool start(IOService *provider);
	virtual void free();
	virtual IOByteCount performRead(IOMemoryDescriptor *dest, UInt64 offset, UInt64 count);
	virtual IOByteCount performWrite(IOMemoryDescriptor *src, UInt64 offset, UInt64 count);
	virtual IOReturn doEjectMedia();
	
	UInt64 getByteCount() const;
	UInt32 getBlockSize() const;
	bool isWritable();
	bool isReady();
};


#endif /* defined(__NBDDisk__NBDBlockService__) */
