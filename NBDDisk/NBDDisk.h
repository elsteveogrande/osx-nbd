//  Copyright (c) 2019-present Steve O'Brien. All rights reserved.
//  See LICENSE in project source root.

#pragma once

class cc_obrien_NBDConnection {
public:
  explicit cc_obrien_NBDConnection(int minor) : minor_(minor) {}

  int minor() const { return minor_; }
  uint64_t id() const { return id_; }

private:
  int const minor_;
  uint64_t id_;
};
