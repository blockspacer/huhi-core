/*  Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stddef.h>

#include "base/macros.h"
#include "base/stl_util.h"
#include "huhi/grit/huhi_theme_resources.h"
#include "build/build_config.h"
#include "chrome/browser/resources_util.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(OS_WIN)
#include "huhi/common/resource_bundle_helper.h"
#include "chrome/grit/chromium_strings.h"
#include "ui/base/l10n/l10n_util.h"
#endif

TEST(HuhiResourcesUtil, CheckIds) {
  const struct {
    const char* name;
    int id;
  } kCases[] = {
    // IDRs from huhi/app/theme/huhi_theme_resources.grd should be valid.
    {"IDR_PRODUCT_LOGO_32_BETA", IDR_PRODUCT_LOGO_32_BETA},
    {"IDR_PRODUCT_LOGO_32_DEV", IDR_PRODUCT_LOGO_32_DEV},
    {"IDR_PRODUCT_LOGO_32_CANARY", IDR_PRODUCT_LOGO_32_CANARY},
    {"IDR_PRODUCT_LOGO_32_DEVELOPMENT", IDR_PRODUCT_LOGO_32_DEVELOPMENT},
  };

  for (size_t i = 0; i < base::size(kCases); ++i)
    EXPECT_EQ(kCases[i].id, ResourcesUtil::GetThemeResourceId(kCases[i].name));
}

#if defined(OS_WIN)
TEST(HuhiResourcesWinTest, CheckStringsForInstaller) {
  // Test whether strings for installer are filled.
  // This test is added because these strings are initially empty and filled
  // with huhi's one.
  // huhi::InitializeResourceBundle can only be called once per unittest
  // running, cannot be called for every test suites. Which will lead to
  // duplicate resources
  // huhi::InitializeResourceBundle();

  EXPECT_FALSE(l10n_util::GetStringUTF16(IDS_SXS_SHORTCUT_NAME).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(IDS_SHORTCUT_NAME_BETA).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(IDS_SHORTCUT_NAME_DEV).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_APP_SHORTCUTS_SUBDIR_NAME_BETA).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_APP_SHORTCUTS_SUBDIR_NAME_DEV).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_INBOUND_MDNS_RULE_NAME_BETA).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_INBOUND_MDNS_RULE_NAME_CANARY).empty());
  EXPECT_FALSE(
      l10n_util::GetStringUTF16(IDS_INBOUND_MDNS_RULE_NAME_DEV).empty());
  EXPECT_FALSE(
      l10n_util::GetStringUTF16(
        IDS_INBOUND_MDNS_RULE_DESCRIPTION_BETA).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_INBOUND_MDNS_RULE_DESCRIPTION_CANARY).empty());
  EXPECT_FALSE(l10n_util::GetStringUTF16(
        IDS_INBOUND_MDNS_RULE_DESCRIPTION_DEV).empty());
}
#endif