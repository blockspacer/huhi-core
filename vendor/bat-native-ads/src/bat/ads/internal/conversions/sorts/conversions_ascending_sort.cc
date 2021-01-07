/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/conversions/sorts/conversions_ascending_sort.h"

#include <algorithm>

namespace ads {

ConversionsAscendingSort::ConversionsAscendingSort() = default;

ConversionsAscendingSort::~ConversionsAscendingSort() = default;

ConversionList ConversionsAscendingSort::Apply(
    const ConversionList& list) const {
  auto sorted_list = list;

  std::sort(sorted_list.begin(), sorted_list.end(),
      [](const ConversionInfo& a, const ConversionInfo& b) {
    return a.type == "postview" && b.type == "postclick";
  });

  return sorted_list;
}

}  // namespace ads
