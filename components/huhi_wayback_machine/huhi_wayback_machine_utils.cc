/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/huhi_wayback_machine/huhi_wayback_machine_utils.h"

#include <string>

#include "base/strings/string_util.h"
#include "net/base/url_util.h"
#include "url/gurl.h"

bool IsWaybackMachineDisabledFor(const GURL& url) {
  if (net::IsLocalhost(url))
    return true;

  if (base::EndsWith(url.host(), ".local", base::CompareCase::SENSITIVE))
    return true;

  if (base::EndsWith(url.host(), ".onion", base::CompareCase::SENSITIVE))
    return true;

  return false;
}
