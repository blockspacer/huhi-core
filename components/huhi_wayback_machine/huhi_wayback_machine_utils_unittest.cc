/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/huhi_wayback_machine/huhi_wayback_machine_utils.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

TEST(HuhiWaybackMachineUtilsTest, LocalHostDisabledTest) {
  EXPECT_TRUE(IsWaybackMachineDisabledFor(GURL("http://localhost/index.html")));
  EXPECT_TRUE(IsWaybackMachineDisabledFor(GURL("http://abcd.local")));
  EXPECT_TRUE(IsWaybackMachineDisabledFor(GURL("http://abcd.onion")));
  EXPECT_TRUE(IsWaybackMachineDisabledFor(GURL("http://127.0.0.1")));
  EXPECT_TRUE(IsWaybackMachineDisabledFor(GURL("http://[::1]")));
  EXPECT_TRUE(IsWaybackMachineDisabledFor(
      GURL("http://127.0045.1.2:8080/index.html")));
  EXPECT_FALSE(IsWaybackMachineDisabledFor(GURL("http://www.local-news.com")));
  EXPECT_FALSE(IsWaybackMachineDisabledFor(GURL("http://www.onion-news.com")));
  EXPECT_FALSE(IsWaybackMachineDisabledFor(GURL("http://www.huhisoft.com")));
}
