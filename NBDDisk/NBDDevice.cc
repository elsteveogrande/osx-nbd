//  Copyright (c) 2019-present Steve O'Brien. All rights reserved.
//  See LICENSE in project source root.

#include "NBDDevice.h"

#include <libkern/c++/OSMetaClass.h>
#include <libkern/c++/OSNumber.h>
#include <miscfs/devfs/devfs.h>

#include <IOKit/IOBSD.h>
#include <IOKit/IOLib.h>

#include "NBDDisk.h"
#include "NBDManager.h"

using super = IOBlockStorageDevice;

bool cc_obrien_NBDDevice::init(OSDictionary* props) {
  if (!super::init(props)) {
    return false;
  }
  auto* minorNumber = OSDynamicCast(OSNumber, props->getObject(kIOBSDMinorKey));
  if (!minorNumber) {
    return false;
  }
  minor_ = minorNumber->unsigned32BitValue();
  return true;
}

bool cc_obrien_NBDDevice::attach(IOService* provider) {
  if (!super::attach(provider)) { return false; }
  manager_ = OSDynamicCast(cc_obrien_NBDManager, provider);
  if (!manager_) { return false; }
  conn_ = new cc_obrien_NBDConnection(minor_);
  if (!(conn_ && conn_->id_)) { re´´turn false; }
  return true;
}

void cc_obrien_NBDDevice::detach(IOService* provider) {
  super::detach(manager_);
  manager_ = nullptr;
}

IOReturn cc_obrien_NBDDevice::doEjectMedia() {
  return kIOReturnUnsupported;
}

IOReturn cc_obrien_NBDDevice::doFormatMedia(uint64_t byteCapacity) {
  return kIOReturnUnsupported;
}

uint32_t
cc_obrien_NBDDevice::doGetFormatCapacities(uint64_t* byteCapacities,
                                           uint32_t capacitiesMaxCount) const {
  if (!byteCapacities) {
    return 1; // indicate we need space for 1 entry
  }
  if (capacitiesMaxCount < 0) {
    return 0; // error
  }
  byteCapacities[0] = getByteCount();
}

char* cc_obrien_NBDDevice::getVendorString() {
  static char const* ret = "VendorFOO";
  return const_cast<char*>(ret);
}

char* cc_obrien_NBDDevice::getProductString() {
  static char const* ret = "ProductFOO";
  return const_cast<char*>(ret);
}

char* cc_obrien_NBDDevice::getRevisionString() {
  static char const* ret = "RevisionFOO";
  return const_cast<char*>(ret);
}

char* cc_obrien_NBDDevice::getAdditionalDeviceInfoString() {
  static char const* ret = "AddlDeviceInfoFOO";
  return const_cast<char*>(ret);
}

IOReturn cc_obrien_NBDDevice::reportBlockSize(uint64_t* blockSize) {
  *blockSize = kBlockSize;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::reportEjectability(bool* isEjectable) {
  *isEjectable = true;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::reportMaxValidBlock(uint64_t* maxBlock) {
  *maxBlock = 0;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::reportMediaState(bool* mediaPresent,
                                               bool* changedState) {
  *mediaPresent = checkAvailable();
  if (changedState != nullptr) {
    *changedState = (prevState_ == *mediaPresent);
  }
  prevState_ = *mediaPresent;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::reportRemovability(bool* isRemovable) {
  *isRemovable = true;
	return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::reportWriteProtection(bool* isWriteProtected) {
  *isWriteProtected = false;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::getWriteCacheState(bool* enabled) {
  *enabled = false;
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::setWriteCacheState(bool enabled) {
  if (enabled) {
    return kIOReturnUnsupported;
  }
  return kIOReturnSuccess;
}

IOReturn cc_obrien_NBDDevice::doAsyncReadWrite(
    IOMemoryDescriptor* buffer, uint64_t block, uint64_t nblks,
    IOStorageAttributes* attributes, IOStorageCompletion* completion) {
  bool mediaPresent;
  reportMediaState(&mediaPresent, nullptr);
  if (!mediaPresent) {
    return kIOReturnNotAttached;
  }

  uint64_t maxBlock;
  reportMaxValidBlock(&maxBlock);
  if ((block + nblks) > maxBlock) {
    return kIOReturnBadArgument;
  }

  IOByteCount actualCount = 0;
  if (buffer->getDirection() == kIODirectionIn) {
    for (int64_t i = 0; i < nblks; i++) {
      if (!readBlock(buffer, block + i)) {
        break;
      }
      actualCount += kBlockSize;
    }
  } else if (buffer->getDirection() == kIODirectionOut) {
    for (int64_t i = 0; i < nblks; i++) {
      if (!writeBlock(buffer, block + i)) {
        break;
      }
      actualCount += kBlockSize;
    }
  } else {
    return kIOReturnBadArgument;
  }

  (completion->action)(completion->target, completion->parameter, kIOReturnSuccess, actualCount);
	return kIOReturnSuccess;
}
