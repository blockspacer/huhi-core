/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_CATEGORY_H_
#define BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_CATEGORY_H_

#include <string>
#include <vector>

#include "bat/ads/result.h"

namespace ads {

struct FilteredCategory {
  FilteredCategory();
  FilteredCategory(
      const FilteredCategory& category);
  ~FilteredCategory();

  std::string ToJson() const;
  Result FromJson(
      const std::string& json);

  std::string name;
};

using FilteredCategoriesList = std::vector<FilteredCategory>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_CATEGORY_H_
