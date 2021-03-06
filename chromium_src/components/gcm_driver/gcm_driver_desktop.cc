/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../components/gcm_driver/gcm_driver_desktop.cc"

namespace gcm {

HuhiGCMDriverDesktop::~HuhiGCMDriverDesktop() = default;

void HuhiGCMDriverDesktop::SetEnabled(bool enabled) {
  enabled_ = enabled;
}

GCMClient::Result HuhiGCMDriverDesktop::EnsureStarted(
  GCMClient::StartMode start_mode) {
  if (!enabled_) {
    return GCMClient::GCM_DISABLED;
  }

  return GCMDriverDesktop::EnsureStarted(start_mode);
}

}  // namespace gcm
