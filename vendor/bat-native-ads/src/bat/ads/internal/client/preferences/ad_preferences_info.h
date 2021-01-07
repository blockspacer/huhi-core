/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_
#define BAT_ADS_INTERNAL_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_

#include <string>

#include "bat/ads/internal/client/preferences/filtered_ad_info.h"
#include "bat/ads/internal/client/preferences/filtered_category_info.h"
#include "bat/ads/internal/client/preferences/flagged_ad_info.h"
#include "bat/ads/internal/client/preferences/saved_ad_info.h"
#include "bat/ads/result.h"

namespace ads {

struct AdPreferencesInfo {
  AdPreferencesInfo();
  AdPreferencesInfo(
      const AdPreferencesInfo& info);
  ~AdPreferencesInfo();

  std::string ToJson() const;
  Result FromJson(
      const std::string& json);

  FilteredAdList filtered_ads;
  FilteredCategoryList filtered_categories;
  SavedAdList saved_ads;
  FlaggedAdList flagged_ads;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_
