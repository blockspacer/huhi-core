/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/download/huhi_download_item_model.h"

#include <string>

#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/common/origin_util.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/text_elider.h"
#include "url/gurl.h"
#include "url/url_constants.h"

using download::DownloadItem;

HuhiDownloadItemModel::HuhiDownloadItemModel(DownloadUIModel* model)
    : model_(model) {}

HuhiDownloadItemModel::~HuhiDownloadItemModel() {}

// Adds origin url to the tooltip text and "Not secure", if needed.
base::string16 HuhiDownloadItemModel::GetTooltipText() {
  base::string16 tooltip = model_->GetTooltipText();

  bool is_secure;
  base::string16 origin_url = GetOriginURLText(&is_secure);

  if (!origin_url.empty()) {
    tooltip += base::ASCIIToUTF16("\n");
    if (!is_secure) {
      tooltip += l10n_util::GetStringUTF16(IDS_NOT_SECURE_VERBOSE_STATE) +
                 base::char16(' ');
    }
    tooltip += origin_url;
  }

  return tooltip;
}

// Returns origin url text and sets |is_secure|.
base::string16 HuhiDownloadItemModel::GetOriginURLText(bool* is_secure) {
  *is_secure = false;
  const GURL gurl = model_->download()->GetURL();
  if (gurl.is_empty()) {
    return base::string16();
  }

  std::string origin;
  if (gurl.is_valid()) {
    *is_secure = content::IsOriginSecure(gurl);
    if (gurl.SchemeIs(url::kAboutScheme)) {
      origin = gurl.spec();
    } else {
      origin = gurl.GetOrigin().spec();
      if (!gurl.SchemeIsFile()) {
        base::TrimString(origin, "/", &origin);
      }
    }
  } else {
    origin = gurl.possibly_invalid_spec();
  }

  return base::UTF8ToUTF16(origin);
}
