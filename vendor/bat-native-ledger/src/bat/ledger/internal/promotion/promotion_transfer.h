/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
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

using GetEligibleTokensCallback =
    std::function<void(type::UnblindedTokenList list)>;

class PromotionTransfer {
 public:
  explicit PromotionTransfer(LedgerImpl* ledger);
  ~PromotionTransfer();

  void GetAmount(ledger::GetTransferableAmountCallback callback);

  void Start(ledger::ResultCallback callback);

 private:
  void GetEligibleTokens(GetEligibleTokensCallback callback);

  void OnGetEligiblePromotions(
      type::PromotionList promotions,
      GetEligibleTokensCallback callback);

  void OnGetEligibleTokens(
      type::UnblindedTokenList list,
      ledger::ResultCallback callback);

  std::vector<type::PromotionType> GetEligiblePromotions();

  LedgerImpl* ledger_;  // NOT OWNED
  std::unique_ptr<credential::Credentials> credentials_;
};

}  // namespace promotion
}  // namespace ledger

#endif  // HUHILEDGER_PROMOTION_TRANSFER_H_
