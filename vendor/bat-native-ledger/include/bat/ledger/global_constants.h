/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_LEDGER_PUBLISHER_INFO_HANDLER_
#define BAT_LEDGER_PUBLISHER_INFO_HANDLER_

#include <string>
#include <vector>
#include <map>
#include <utility>

#include "bat/ledger/export.h"

namespace ledger {
namespace constant {

const char kWalletAnonymous[] = "anonymous";
const char kWalletUphold[] = "uphold";
const char kWalletUnBlinded[] = "blinded";

}  // namespace constant
}  // namespace ledger

#endif  // BAT_LEDGER_PUBLISHER_INFO_HANDLER_
