// Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Huhi Software
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "extensions/common/constants.h"

// Avoid a jump from small image to large image when the non-default
// image is set.
#define HUHI_GET_EXTENSION_ACTION                     \
      (extension.id() == huhi_rewards_extension_id || \
       extension.id() == huhi_extension_id) ?         \
        kHuhiActionGraphicSize :

#include "../../../../extensions/browser/extension_action_manager.cc"
#undef HUHI_GET_EXTENSION_ACTION
