/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_WIN_H_
#define BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_WIN_H_

#include <string>

#include "bat/ads/internal/platform/platform_helper.h"

namespace ads {

class PlatformHelperWin : public PlatformHelper {
 public:
  PlatformHelperWin(const PlatformHelperWin&) = delete;
  PlatformHelperWin& operator=(const PlatformHelperWin&) = delete;

  static PlatformHelperWin* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<PlatformHelperWin>;

  PlatformHelperWin();
  ~PlatformHelperWin() override;

  // PlatformHelper impl
  bool IsMobile() const override;
  std::string GetPlatformName() const override;
  PlatformType GetPlatform() const override;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_WIN_H_
