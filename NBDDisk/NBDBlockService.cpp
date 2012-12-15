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

#define super IOService


void cc_obrien_NBDBlockService::setSizeMB(int sizeMB)
{
	this->size = ((UInt64) 1048576) * ((UInt64) sizeMB);
}


UInt64 cc_obrien_NBDBlockService::getByteCount() const
{
	return this->size;
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
	IOLog("block svc starting\n");
	bool ret = false;
	this->memory = NULL;
	this->buffer = NULL;
	this->nub = NULL;

	if(! super::start(provider))
	{
		goto abort;
	}
	
	this->memory = IOBufferMemoryDescriptor::withCapacity(this->getByteCount(), kIODirectionInOut);
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
	IOLog("block svc aborting\n");
	if(this->memory)
	{
		this->memory->release();
		this->memory = NULL;
	}

out:
	IOLog("block svc started\n");
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

	if(this->nub)
	{
		this->nub->release();
		this->nub = NULL;
	}

	IOLog("nbdblockservice: freeing super\n");
	super::free();
}


IOReturn cc_obrien_NBDBlockService::doEjectMedia()
{
	IOLog("block svc ejecting\n");
	if(this->nub)
	{
		this->nub->detach(this);
		this->nub->stop(this);
		this->nub->release();
		this->nub = NULL;
	}

	this->detachFromParent(IORegistryEntry::getRegistryRoot(), gIOServicePlane);
	
	IOLog("block svc ejected\n");
	return kIOReturnSuccess;
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
	
	this->nub = new cc_obrien_NBDDiskStorageDevice();
	if(! this->nub)
	{
		goto abort;
	}
	
	if(! this->nub->init(NULL))
	{
		goto abort;
	}
	
	if(! this->nub->attach(this))
	{
		goto abort;
	}
	
	this->nub->registerService();

	ret = true;
	goto out;
	
abort:
	/* nothing special to do when aborting */

out:
	/* note; have to unconditionally un-retain this nub.  The OS itself will retain it */

	return ret;
}

