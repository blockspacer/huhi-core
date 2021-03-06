/* Copyright 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/test/base/huhi_test_launcher_delegate.h"

#include "huhi/app/huhi_main_delegate.h"

HuhiTestLauncherDelegate::HuhiTestLauncherDelegate(
    ChromeTestSuiteRunner* runner)
    : ChromeTestLauncherDelegate(runner) {}

HuhiTestLauncherDelegate::~HuhiTestLauncherDelegate() = default;

content::ContentMainDelegate*
HuhiTestLauncherDelegate::CreateContentMainDelegate() {
  return new HuhiMainDelegate(base::TimeTicks::Now());
}
