/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FLAGGED_AD_INFO_H_
#define BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FLAGGED_AD_INFO_H_

#include <string>
#include <vector>

#include "bat/ads/result.h"

namespace ads {

struct FlaggedAdInfo {
  FlaggedAdInfo();
  FlaggedAdInfo(
      const FlaggedAdInfo& ad);
  ~FlaggedAdInfo();

  std::string ToJson() const;
  Result FromJson(
      const std::string& json);

  std::string creative_instance_id;
  std::string creative_set_id;
};

using FlaggedAdList = std::vector<FlaggedAdInfo>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FLAGGED_AD_INFO_H_
