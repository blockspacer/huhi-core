/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHILEDGER_PROMOTION_TRANSFER_H_
#define HUHILEDGER_PROMOTION_TRANSFER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bat/ledger/internal/credentials/credentials_factory.h"
#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace promotion {

class PromotionTransfer {
 public:
  explicit PromotionTransfer(LedgerImpl* ledger);
  ~PromotionTransfer();

  void Start(ledger::ResultCallback callback);

 private:
  void GetEligibleTokens(
      type::PromotionList promotions,
      ledger::ResultCallback callback);

  void OnGetEligibleTokens(
      type::UnblindedTokenList list,
      ledger::ResultCallback callback);

  LedgerImpl* ledger_;  // NOT OWNED
  std::unique_ptr<credential::Credentials> credentials_;
};

}  // namespace promotion
}  // namespace ledger

#endif  // HUHILEDGER_PROMOTION_TRANSFER_H_
