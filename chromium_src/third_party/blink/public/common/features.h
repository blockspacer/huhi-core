/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_
#define HUHI_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_

#include "base/feature_list.h"
#include "third_party/blink/public/common/common_export.h"

namespace blink {
namespace features {

BLINK_COMMON_EXPORT extern const base::Feature kHuhiEphemeralStorage;

}  // namespace features
}  // namespace blink

#include "../../../../../../third_party/blink/public/common/features.h"

#undef HUHI_FEATURES_H

#endif  // HUHI_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_
