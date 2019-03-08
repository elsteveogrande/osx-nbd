//  Copyright (c) 2019-present Steve O'Brien. All rights reserved.
//  See LICENSE in project source root.

#pragma once

#include <stdint.h>

#include <IOKit/IOService.h>

#include "NBDDevice.h"

/**
 * The manager for NBD devices.
 *
 * Upon initialization this builds (kDeviceCount = 10) devices.
 * Each device is accessible via dev entries with major number (kMajor = 92)
 * and minor numbers 0 to (kDeviceCount - 1).
 */
class cc_obrien_NBDManager : public IOService {
  OSDeclareDefaultStructors(cc_obrien_NBDManager);

public:
  bool init(OSDictionary* props = nullptr) override;
  IOService* probe(IOService* provider, int32_t* score) override;
  bool start(IOService* provider) override;
  void stop(IOService* provider) override;
  void free() override;

private:
  static constexpr uint8_t const kMajor = 92;
  static constexpr uint8_t const kDeviceCount = 10;

  cc_obrien_NBDDevice* devices_[kDeviceCount]{nullptr};
};
