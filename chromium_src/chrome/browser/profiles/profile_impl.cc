/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/profiles/huhi_bookmark_model_loaded_observer.h"
#include "huhi/browser/profiles/huhi_profile_impl.h"
#define BookmarkModelLoadedObserver HuhiBookmarkModelLoadedObserver
#include "../../../../../chrome/browser/profiles/profile_impl.cc"
#undef BookmarkModelLoadedObserver
