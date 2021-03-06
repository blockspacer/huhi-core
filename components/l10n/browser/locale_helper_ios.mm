/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/l10n/browser/locale_helper_ios.h"

#import <Foundation/Foundation.h>

namespace huhi_l10n {

LocaleHelperIos::LocaleHelperIos() = default;

LocaleHelperIos::~LocaleHelperIos() = default;

std::string LocaleHelperIos::GetLocale() const {
  NSString *locale = [[NSLocale preferredLanguages] firstObject];
  return locale.UTF8String;
}

LocaleHelperIos* LocaleHelperIos::GetInstanceImpl() {
  return base::Singleton<LocaleHelperIos>::get();
}

LocaleHelper* LocaleHelper::GetInstanceImpl() {
  return LocaleHelperIos::GetInstanceImpl();
}

}  // namespace huhi_l10n
