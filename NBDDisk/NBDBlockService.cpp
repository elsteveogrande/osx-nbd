//
//  NBDBlockService.h
//
//  Created by Steve O'Brien on 12/8/12.
//  Copyright (c) 2012 Steve O'Brien. All rights reserved.
//

#include "NBDBlockService.h"
#include "NBDDiskStorageDevice.h"

OSDefineMetaClassAndStructors(cc_obrien_NBDBlockService, IOService)

#define FIXED_BLOCK_SIZE (512)
#define FIXED_CACHE_SIZE (16777216)

#define super IOService


UInt64 cc_obrien_NBDBlockService::getByteCount()
{
	return FIXED_CACHE_SIZE;
}


UInt32 cc_obrien_NBDBlockService::getBlockSize()
{
	return FIXED_BLOCK_SIZE;
}


bool cc_obrien_NBDBlockService::isWritable()
{
	return true;
}


bool cc_obrien_NBDBlockService::isReady()
{
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
	
	this->memory = IOBufferMemoryDescriptor::withCapacity(FIXED_CACHE_SIZE, kIODirectionOutIn);
	if(! this->memory)
	{
		goto abort;
	}
	
	this->buffer = this->memory->getBytesNoCopy();
	
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
	}
	
out:
	return ret;
}


void cc_obrien_NBDBlockService::free()
{
	if(this->memory)
	{
		this->memory->release();
	}
	
	super::free();
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
	return true;
	
abort:
	if(nub)
	{
		nub->release();
	}
	
out:
	return ret;
}

