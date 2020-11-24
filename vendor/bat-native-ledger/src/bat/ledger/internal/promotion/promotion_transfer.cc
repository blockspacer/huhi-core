/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/logging/event_log_keys.h"
#include "bat/ledger/internal/promotion/promotion_transfer.h"
#include "bat/ledger/internal/promotion/promotion_util.h"
#include "bat/ledger/internal/constants.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace ledger {
namespace promotion {

PromotionTransfer::PromotionTransfer(LedgerImpl* ledger) :
    ledger_(ledger) {
  DCHECK(ledger_);
  credentials_ = credential::CredentialsFactory::Create(
      ledger_,
      type::CredsBatchType::PROMOTION);
  DCHECK(credentials_);
}

PromotionTransfer::~PromotionTransfer() = default;

void PromotionTransfer::Start(ledger::ResultCallback callback) {
  auto tokens_callback = std::bind(&PromotionTransfer::GetEligibleTokens,
      this,
      _1,
      callback);

  ledger_->database()->GetPromotionListByType(
      GetEligiblePromotions(),
      tokens_callback);
}

void PromotionTransfer::GetEligibleTokens(
    type::PromotionList promotions,
    ledger::ResultCallback callback) {
  auto tokens_callback = std::bind(&PromotionTransfer::OnGetEligibleTokens,
      this,
      _1,
      callback);

  std::vector<std::string> ids;
  for (auto& promotion : promotions) {
    if (!promotion) {
      continue;
    }

    ids.push_back(promotion->id);
  }

  ledger_->database()->GetSpendableUnblindedTokensByTriggerIds(
      ids,
      tokens_callback);
}

void PromotionTransfer::OnGetEligibleTokens(
    type::UnblindedTokenList list,
    ledger::ResultCallback callback) {
  if (list.empty()) {
    BLOG(1, "No eligible tokens");
    callback(type::Result::LEDGER_OK);
    return;
  }

  std::vector<type::UnblindedToken> token_list;
  for (auto& item : list) {
    token_list.push_back(*item);
  }

  credential::CredentialsRedeem redeem;
  redeem.type = type::RewardsType::TRANSFER;
  redeem.processor = type::ContributionProcessor::HUHI_TOKENS;
  redeem.token_list = token_list;

  const double transfer_amount =
      token_list.size() * constant::kVotePrice;
  credentials_->RedeemTokens(
      redeem,
      [this, transfer_amount, callback](const type::Result result) {
        if (result == type::Result::LEDGER_OK) {
            ledger_->database()->SaveEventLog(
                log::kPromotionsClaimed,
                std::to_string(transfer_amount));
        }
        callback(result);
      });
}

}  // namespace promotion
}  // namespace ledger
