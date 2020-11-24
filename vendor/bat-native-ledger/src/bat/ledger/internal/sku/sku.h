/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHILEDGER_SKU_H_
#define HUHILEDGER_SKU_H_

#include <map>
#include <string>
#include <vector>

#include "bat/ledger/ledger.h"

namespace ledger {
namespace sku {

class SKU {
 public:
  virtual ~SKU() = default;

  virtual void Retry(
      const std::string& order_id,
      type::ExternalWalletPtr wallet,
      ledger::SKUOrderCallback callback) = 0;

  virtual void Process(
      const std::vector<type::SKUOrderItem>& items,
      type::ExternalWalletPtr wallet,
      ledger::SKUOrderCallback callback,
      const std::string& contribution_id = "") = 0;
};

}  // namespace sku
}  // namespace ledger

#endif  // HUHILEDGER_SKU_H_
