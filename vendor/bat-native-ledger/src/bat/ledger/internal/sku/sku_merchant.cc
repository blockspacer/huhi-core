/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/global_constants.h"
#include "bat/ledger/internal/sku/sku_util.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/sku/sku_merchant.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace ledger {
namespace sku {

SKUMerchant::SKUMerchant(LedgerImpl* ledger) :
    ledger_(ledger),
    common_(std::make_unique<SKUCommon>(ledger)) {
  DCHECK(ledger_);
}

SKUMerchant::~SKUMerchant() = default;

void SKUMerchant::Process(
    const std::vector<type::SKUOrderItem>& items,
    type::ExternalWalletPtr wallet,
    ledger::SKUOrderCallback callback,
    const std::string& contribution_id) {
  if (!wallet) {
    BLOG(0, "Wallet is null");
    callback(type::Result::LEDGER_ERROR, "");
    return;
  }

  auto create_callback = std::bind(&SKUMerchant::OrderCreated,
      this,
      _1,
      _2,
      *wallet,
      callback);

  common_->CreateOrder(items, create_callback);
}

void SKUMerchant::OrderCreated(
    const type::Result result,
    const std::string& order_id,
    const type::ExternalWallet& wallet,
    ledger::SKUOrderCallback callback) {
  if (result != type::Result::LEDGER_OK) {
    BLOG(0, "Order was not successful");
    callback(result, "");
    return;
  }

  auto get_callback = std::bind(&SKUMerchant::OnOrder,
      this,
      _1,
      wallet,
      callback);

  ledger_->database()->GetSKUOrder(order_id, get_callback);
}

void SKUMerchant::OnOrder(
    type::SKUOrderPtr order,
    const type::ExternalWallet& wallet,
    ledger::SKUOrderCallback callback) {
  if (!order) {
    BLOG(0, "Order is null");
    callback(type::Result::LEDGER_ERROR, "");
    return;
  }

  if (wallet.type == constant::kWalletUphold) {
    auto publisher_callback =
        std::bind(&SKUMerchant::OnServerPublisherInfo,
          this,
          _1,
          std::make_shared<type::SKUOrderPtr>(order->Clone()),
          wallet,
          callback);

    ledger_->publisher()->GetServerPublisherInfo(
        order->merchant_id,
        publisher_callback);
    return;
  }

  common_->CreateTransaction(std::move(order), "", wallet, callback);
}

void SKUMerchant::OnServerPublisherInfo(
    type::ServerPublisherInfoPtr info,
    std::shared_ptr<type::SKUOrderPtr> shared_order,
    const type::ExternalWallet& wallet,
    ledger::SKUOrderCallback callback) {
  if (!shared_order || !info) {
    BLOG(0, "Order/Publisher not found");
    callback(type::Result::LEDGER_ERROR, "");
    return;
  }

  if (info->address.empty()) {
    BLOG(0, "Publisher address is empty");
    callback(type::Result::LEDGER_ERROR, "");
    return;
  }

  common_->CreateTransaction(
      std::move(*shared_order),
      info->address,
      wallet,
      callback);
}

void SKUMerchant::Retry(
    const std::string& order_id,
    type::ExternalWalletPtr wallet,
    ledger::SKUOrderCallback callback) {
  // We will implement retry logic when we will have more complex flows,
  // right now there is nothing to retry
  NOTREACHED();
}

}  // namespace sku
}  // namespace ledger
