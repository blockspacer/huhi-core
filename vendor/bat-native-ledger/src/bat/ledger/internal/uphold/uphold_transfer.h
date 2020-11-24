/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHILEDGER_UPHOLD_UPHOLD_TRANSFER_H_
#define HUHILEDGER_UPHOLD_UPHOLD_TRANSFER_H_

#include <map>
#include <memory>
#include <string>

#include "bat/ledger/ledger.h"
#include "bat/ledger/internal/uphold/uphold.h"

namespace ledger {
class LedgerImpl;

namespace endpoint {
class UpholdServer;
}

namespace uphold {

class UpholdTransfer {
 public:
  explicit UpholdTransfer(LedgerImpl* ledger, Uphold* uphold);

  ~UpholdTransfer();

  void Start(
      const Transaction& transaction,
      client::TransactionCallback callback);

 private:
  void OnCreateTransaction(
      const type::Result result,
      const std::string& id,
      client::TransactionCallback callback);

  void CommitTransaction(
      const std::string& transaction_id,
      client::TransactionCallback callback);

  void OnCommitTransaction(
      const type::Result result,
      const std::string& transaction_id,
      client::TransactionCallback callback);

  LedgerImpl* ledger_;  // NOT OWNED
  Uphold* uphold_;  // NOT OWNED
  std::unique_ptr<endpoint::UpholdServer> uphold_server_;
};

}  // namespace uphold
}  // namespace ledger
#endif  // HUHILEDGER_UPHOLD_UPHOLD_TRANSFER_H_
