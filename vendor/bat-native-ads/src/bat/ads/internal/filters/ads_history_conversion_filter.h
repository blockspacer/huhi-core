/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_FILTERS_ADS_HISTORY_CONVERSION_FILTER_H_
#define BAT_ADS_INTERNAL_FILTERS_ADS_HISTORY_CONVERSION_FILTER_H_

#include <deque>

#include "bat/ads/confirmation_type.h"
#include "bat/ads/internal/filters/ads_history_filter.h"

namespace ads {

class AdsHistoryConversionFilter : public AdsHistoryFilter {
 public:
  AdsHistoryConversionFilter();
  ~AdsHistoryConversionFilter() override;

  std::deque<AdHistory> Apply(
      const std::deque<AdHistory>& history) const override;

 private:
  bool ShouldFilterConfirmationType(
      const ConfirmationType& type) const;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_FILTERS_ADS_HISTORY_CONVERSION_FILTER_H_
