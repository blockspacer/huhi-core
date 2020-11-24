/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_PROFILES_PROFILE_UTIL_H_
#define HUHI_BROWSER_PROFILES_PROFILE_UTIL_H_

#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/supports_user_data.h"
// this needs to be here so we don't accidentally override the real method
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_context.h"

namespace huhi {

Profile* CreateParentProfileData(content::BrowserContext* context);

base::FilePath GetParentProfilePath(content::BrowserContext* context);

base::FilePath GetParentProfilePath(const base::FilePath& file_path);

bool IsSessionProfile(content::BrowserContext* context);

bool IsSessionProfilePath(const base::FilePath& path);

bool IsTorProfilePath(const base::FilePath& path);

bool IsTorProfile(content::BrowserContext* context);

Profile* GetParentProfile(content::BrowserContext* context);

Profile* GetParentProfile(const base::FilePath& path);

// This function is similar to Profile::IsGuestSession. It is used to get
// around the bug(?) in OffTheRecordProfileImpl::Init() code that calls
// set_is_guest_profile on off-the-record profile only after calling
// BrowserContextDependencyManager::CreateBrowserContextServices. Because of
// this, in search_engine_provider_service_factory.cc (for example), in
// InitializeSearchEngineProviderServiceIfNeeded we can't correctly identify
// the guest profile by calling IsGuestSession and have to use this function.
bool IsGuestProfile(content::BrowserContext* profile);

bool IsTorDisabledForProfile(Profile* profile);

}  // namespace huhi

namespace chrome {

// Get the correct profile for keyed services that use
// GetBrowserContextRedirectedInIncognito or equivalent
content::BrowserContext* GetBrowserContextRedirectedInIncognitoOverride(
    content::BrowserContext* context);

}  // namespace chrome

// Get the correct profile for keyed services that do NOT use
// GetBrowserContextRedirectedInIncognito or equivalent
#define HUHI_GET_BROWSER_CONTEXT_TO_USE_WITHOUT_REDIRECT                      \
  if (huhi::IsSessionProfile(context)) {                                      \
    auto* parent = huhi::GetParentProfile(context);                           \
    context =                                                                  \
        context->IsOffTheRecord() ? parent->GetOffTheRecordProfile() : parent; \
  }

#endif  // HUHI_BROWSER_PROFILES_PROFILE_UTIL_H_