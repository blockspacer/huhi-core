/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/common/huhi_channel_info.h"

#include "chrome/common/channel_info.h"

namespace huhi {

std::string GetChannelName() {
#if defined(OFFICIAL_BUILD)
  std::string channel_name = chrome::GetChannelName();
  if (channel_name.empty()) {
    channel_name = "release";
  }

  return channel_name;
#else  // OFFICIAL_BUILD
  return "developer";
#endif  // !OFFICIAL_BUILD
}

}  // namespace huhi
