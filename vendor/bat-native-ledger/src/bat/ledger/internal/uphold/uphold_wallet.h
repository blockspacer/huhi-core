/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHILEDGER_UPHOLD_UPHOLD_WALLET_H_
#define HUHILEDGER_UPHOLD_UPHOLD_WALLET_H_

#include <string>

#include "bat/ledger/ledger.h"
#include "bat/ledger/internal/uphold/uphold.h"

namespace ledger {
class LedgerImpl;

namespace uphold {

class UpholdWallet {
 public:
  explicit UpholdWallet(LedgerImpl* ledger, Uphold* uphold);

  ~UpholdWallet();

  void Generate(ledger::ResultCallback callback);

 private:
  void OnGenerate(
      const type::Result result,
      const User& user,
      ledger::ResultCallback callback);

  void OnCreateCard(
      const type::Result result,
      const std::string& address,
      ledger::ResultCallback callback);

  type::WalletStatus GetNewStatus(
      const type::WalletStatus old_status,
      const User& user);

  LedgerImpl* ledger_;  // NOT OWNED
  Uphold* uphold_;  // NOT OWNED
};

}  // namespace uphold
}  // namespace ledger
#endif  // HUHILEDGER_UPHOLD_UPHOLD_WALLET_H_
