//
//  NBDBlockService.h
//
//  Created by Steve O'Brien on 12/8/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#include <IOKit/IOLib.h>
#include "NBDBlockService.h"
#include "NBDDiskStorageDevice.h"

OSDefineMetaClassAndStructors(cc_obrien_NBDBlockService, IOService)

#define FIXED_BLOCK_SIZE (512)
#define FIXED_CACHE_SIZE (16777216)

#define super IOService


UInt64 cc_obrien_NBDBlockService::getByteCount() const
{
	return FIXED_CACHE_SIZE;
}


UInt32 cc_obrien_NBDBlockService::getBlockSize() const
{
	return FIXED_BLOCK_SIZE;
}


bool cc_obrien_NBDBlockService::isWritable()
{
	return true;
}


bool cc_obrien_NBDBlockService::isReady()
{
	// currently this only implements RAM device; it's always ready!
	return true;
}


bool cc_obrien_NBDBlockService::start(IOService *provider)
{
	bool ret = false;
	this->memory = NULL;
	this->buffer = NULL;

	if(! super::start(provider))
	{
		goto abort;
	}
	
	this->memory = IOBufferMemoryDescriptor::withCapacity(FIXED_CACHE_SIZE, kIODirectionInOut);
	if(! this->memory)
	{
		goto abort;
	}
	
	this->buffer = this->memory->getBytesNoCopy();
	if(! this->buffer)
	{
		goto abort;
	}
	
	if(! this->buildDevice())
	{
		goto abort;
	}

	ret = true;
	goto out;

abort:
	if(this->memory)
	{
		this->memory->release();
		this->memory = NULL;
	}

out:
	return ret;
}


void cc_obrien_NBDBlockService::free()
{
	IOLog("nbdblockservice: freeing this\n");

	if(this->memory)
	{
		this->memory->release();
		this->memory = NULL;
	}

	IOLog("nbdblockservice: freeing super\n");
	super::free();

	IOLog("nbdblockservice: freed\n");
}


IOByteCount cc_obrien_NBDBlockService::performRead(IOMemoryDescriptor *dest, UInt64 offset, UInt64 count)
{
	return dest->writeBytes(0, (void *)(((UInt64) (this->buffer)) + offset), count);
}


IOByteCount cc_obrien_NBDBlockService::performWrite(IOMemoryDescriptor *src, UInt64 offset, UInt64 count)
{
	return src->readBytes(0, (void *)(((UInt64) (this->buffer)) + offset), count);
}


bool cc_obrien_NBDBlockService::buildDevice()
{
	bool ret = false;
	cc_obrien_NBDDiskStorageDevice *nub = NULL;
	
	nub = new cc_obrien_NBDDiskStorageDevice();
	if(! nub)
	{
		goto abort;
	}
	
	if(! nub->init(NULL))
	{
		goto abort;
	}
	
	if(! nub->attach(this))
	{
		goto abort;
	}
	
	nub->registerService();

	ret = true;
	goto out;
	
abort:
	/* nothing special to do when aborting */

out:
	/* note; have to unconditionally un-retain this nub.  The OS itself will retain it */
	if(nub)
	{
		nub->release();
	}

	return ret;
}

