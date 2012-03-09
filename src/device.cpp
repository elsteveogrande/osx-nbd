#include "device.h"


IOReturn NBDBlockStorageDevice::doEjectMedia()
{
}


IOReturn NBDBlockStorageDevice::doFormatMedia(UInt64)
{
}


UInt32 NBDBlockStorageDevice::doGetFormatCapacities(UInt64*, UInt32) const
{
}


IOReturn NBDBlockStorageDevice::doSynchronizeCache()
{
}


char* NBDBlockStorageDevice::getVendorString()
{
}


char* NBDBlockStorageDevice::getProductString()
{
}


char* NBDBlockStorageDevice::getRevisionString()
{
}


char* NBDBlockStorageDevice::getAdditionalDeviceInfoString()
{
}


IOReturn NBDBlockStorageDevice::reportBlockSize(UInt64*)
{
}


IOReturn NBDBlockStorageDevice::reportEjectability(bool*)
{
}


IOReturn NBDBlockStorageDevice::reportMaxValidBlock(UInt64*)
{
}


IOReturn NBDBlockStorageDevice::reportMediaState(bool*, bool*)
{
}


IOReturn NBDBlockStorageDevice::reportRemovability(bool*)
{
}


IOReturn NBDBlockStorageDevice::reportWriteProtection(bool*)
{
}


IOReturn NBDBlockStorageDevice::getWriteCacheState(bool*)
{
}


IOReturn NBDBlockStorageDevice::setWriteCacheState(bool)
{
}


IOReturn NBDBlockStorageDevice::doAsyncReadWrite(IOMemoryDescriptor*, UInt64, UInt64, IOStorageAttributes*, IOStorageCompletion*)
{
}



void NBDBlockStorageDevice::stop()
{
}
