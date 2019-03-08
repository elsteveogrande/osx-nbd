//  Copyright (c) 2019-present Steve O'Brien. All rights reserved.
//  See LICENSE in project source root.

#pragma once

#include <stdint.h>

#include <IOKit/storage/IOBlockStorageDevice.h>

class cc_obrien_NBDManager;

class cc_obrien_NBDDevice : public IOBlockStorageDevice {
  OSDeclareDefaultStructors(cc_obrien_NBDDevice);

public:
  bool init(OSDictionary* props) override;
  bool attach(IOService* provider) override;
  void detach(IOService* provider) override;
  IOReturn doEjectMedia() override;
  IOReturn doFormatMedia(uint64_t byteCapacity) override;
  uint32_t doGetFormatCapacities(uint64_t* byteCapacity,
                                 uint32_t capacitiesMaxCount) const override;
  char* getVendorString() override;
  char* getProductString() override;
  char* getRevisionString() override;
  char* getAdditionalDeviceInfoString() override;
  IOReturn reportBlockSize(uint64_t* blockSize) override;
  IOReturn reportEjectability(bool* isEjectable) override;
  IOReturn reportMaxValidBlock(uint64_t* maxBlock) override;
  IOReturn reportMediaState(bool* mediaPresent, bool* changedState) override;
  IOReturn reportRemovability(bool* isRemovable) override;
  IOReturn reportWriteProtection(bool* isWriteProtected) override;
  IOReturn getWriteCacheState(bool* enabled) override;
  IOReturn setWriteCacheState(bool enabled) override;
  IOReturn doAsyncReadWrite(IOMemoryDescriptor* buffer, uint64_t block,
                            uint64_t nblks, IOStorageAttributes* attributes,
                            IOStorageCompletion* completion) override;

private:
  static constexpr size_t const kBlockSize{1024};
  uint32_t minor_;
  cc_obrien_NBDManager* manager_{nullptr};
  cc_obrien_NBDConnection* conn_{nullptr};
};
