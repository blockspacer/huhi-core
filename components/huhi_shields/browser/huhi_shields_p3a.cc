/* Copyright 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/huhi_shields/browser/huhi_shields_p3a.h"

#include "base/logging.h"
#include "base/metrics/histogram_macros.h"
#include "huhi/components/p3a/huhi_p3a_utils.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace huhi_shields {

namespace {
// TODO(iefremov): Move to separate header when needed.
constexpr char kPrefName[] = "huhi_shields.p3a_usage";
}  // namespace

void MaybeRecordShieldsUsageP3A(ShieldsIconUsage usage,
                                PrefService* local_state) {
  ::huhi::RecordValueIfGreater<ShieldsIconUsage>(
      usage, "Huhi.Shields.UsageStatus", kPrefName, local_state);
}

void RegisterShieldsP3APrefs(PrefRegistrySimple* local_state) {
  local_state->RegisterIntegerPref(kPrefName, -1);
}

}  // namespace huhi_shields
