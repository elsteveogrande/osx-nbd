//  Copyright (c) 2019-present Steve O'Brien. All rights reserved.
//  See LICENSE in project source root.

#include "NBDManager.h"

#include <libkern/c++/OSNumber.h>
#include <libkern/c++/OSString.h>
#include <miscfs/devfs/devfs.h>

#include <IOKit/IOBSD.h>
#include <IOKit/IOLib.h>

OSDefineMetaClassAndStructors(cc_obrien_NBDManager, IOService);

bool cc_obrien_NBDManager::init(OSDictionary* props) {
  return IOService::init(props);
}

IOService* cc_obrien_NBDManager::probe(IOService* provider, int32_t* score) {
  return IOService::probe(provider, score);
}

bool cc_obrien_NBDManager::start(IOService* provider) {
  if (!IOService::start(provider)) {
    return false;
  }

  attachToParent(IORegistryEntry::getRegistryRoot(), gIOServicePlane);
  registerService();

  auto* dev = new cc_obrien_NBDDevice();
  auto* devProps = new OSDictionary();
  devProps->setObject(kIOBSDNameKey, OSString::withCString("nbd"));
  devProps->setObject(kIOBSDMajorKey, OSNumber::withNumber(kMajor, 32));
  devProps->setObject(kIOBSDMinorKey, OSNumber::withNumber(0ULL, 32));
  if (!dev->init(devProps)) { goto error; }
  if (!dev->attach(this)) { goto error; }
  dev->registerService();
  devices_[0] = dev;
  return true;

error:
  dev->release();
  return false;
}

void cc_obrien_NBDManager::stop(IOService* provider) {
  for (auto i = 0; i < kDeviceCount; i++) {
    if (devices_[i]) {
      devices_[i]->release();
    }
  }

  detachFromParent(IORegistryEntry::getRegistryRoot(), gIOServicePlane);
  IOService::stop(provider);
}

void cc_obrien_NBDManager::free() {
  IOService::free();
}
