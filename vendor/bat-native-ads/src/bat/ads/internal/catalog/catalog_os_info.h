/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CATALOG_CATALOG_OS_INFO_H_
#define BAT_ADS_INTERNAL_CATALOG_CATALOG_OS_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct CatalogOsInfo {
  std::string code;
  std::string name;
};

using CatalogOsList = std::vector<CatalogOsInfo>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CATALOG_CATALOG_OS_INFO_H_
