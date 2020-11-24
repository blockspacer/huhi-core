/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/profiles/huhi_profile_avatar_downloader.h"

#include <string>
#include <utility>

#include "base/files/file_path.h"
#include "ui/gfx/image/image.h"

namespace {
void DummyCallback(gfx::Image image,
                   const std::string& key,
                   const base::FilePath& image_path) {}
}  // namespace

HuhiProfileAvatarDownloader::HuhiProfileAvatarDownloader(
    size_t icon_index,
    FetchCompleteCallback callback)
    : ProfileAvatarDownloader(icon_index, base::BindOnce(DummyCallback)),
      callback_(std::move(callback)) {}

void HuhiProfileAvatarDownloader::Start() {
  std::move(callback_).Run(gfx::Image(), "", base::FilePath());
}

HuhiProfileAvatarDownloader::~HuhiProfileAvatarDownloader() {}