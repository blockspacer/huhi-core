/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "../../../../../../../chrome/browser/ui/views/download/download_item_view.cc"

bool DownloadItemView::IsShowingWarningDialog() const {
  return is_download_warning(mode_);
}
