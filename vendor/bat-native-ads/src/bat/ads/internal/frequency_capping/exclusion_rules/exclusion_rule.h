/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_H_
#define BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_H_

#include <string>

#include "bat/ads/internal/bundle/creative_ad_info.h"

namespace ads {

class ExclusionRule {
 public:
  virtual ~ExclusionRule() = default;

  virtual bool ShouldExclude(
      const CreativeAdInfo& ad) = 0;

  virtual std::string get_last_message() const = 0;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_H_
