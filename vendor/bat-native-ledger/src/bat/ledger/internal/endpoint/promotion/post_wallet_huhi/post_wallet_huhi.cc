/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "bat/ledger/internal/endpoint/promotion/post_wallet_huhi/post_wallet_huhi.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/strings/stringprintf.h"
#include "bat/ledger/internal/common/security_util.h"
#include "bat/ledger/internal/endpoint/promotion/promotions_util.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/common/request_util.h"
#include "net/http/http_status_code.h"

using std::placeholders::_1;

namespace ledger {
namespace endpoint {
namespace promotion {

PostWalletHuhi::PostWalletHuhi(LedgerImpl* ledger):
    ledger_(ledger) {
  DCHECK(ledger_);
}

PostWalletHuhi::~PostWalletHuhi() = default;

std::string PostWalletHuhi::GetUrl() {
  return GetServerUrl("/v3/wallet/huhi");
}

type::Result PostWalletHuhi::CheckStatusCode(const int status_code) {
  if (status_code == net::HTTP_BAD_REQUEST) {
    BLOG(0, "Invalid request");
    return type::Result::LEDGER_ERROR;
  }

  if (status_code == net::HTTP_SERVICE_UNAVAILABLE) {
    BLOG(0, "No conversion rate yet in ratios service");
    return type::Result::BAD_REGISTRATION_RESPONSE;
  }

  if (status_code != net::HTTP_CREATED) {
    return type::Result::LEDGER_ERROR;
  }

  return type::Result::LEDGER_OK;
}

type::Result PostWalletHuhi::ParseBody(
    const std::string& body,
    std::string* payment_id) {
  DCHECK(payment_id);

  base::Optional<base::Value> value = base::JSONReader::Read(body);
  if (!value || !value->is_dict()) {
    BLOG(0, "Invalid JSON");
    return type::Result::LEDGER_ERROR;
  }

  base::DictionaryValue* dictionary = nullptr;
  if (!value->GetAsDictionary(&dictionary)) {
    BLOG(0, "Invalid JSON");
    return type::Result::LEDGER_ERROR;
  }

  const auto* payment_id_string = dictionary->FindStringKey("paymentId");
  if (!payment_id_string || payment_id_string->empty()) {
    BLOG(1, "Payment id is wrong");
    return type::Result::LEDGER_ERROR;
  }

  *payment_id = *payment_id_string;

  return type::Result::LEDGER_OK;
}

void PostWalletHuhi::Request(
    PostWalletHuhiCallback callback) {
  const auto seed = ledger_->state()->GetRecoverySeed();
  const auto headers = util::BuildSignHeaders(
      "post /v3/wallet/huhi",
      "",
      util::Security::GetPublicKeyHexFromSeed(seed),
      seed);

  auto url_callback = std::bind(&PostWalletHuhi::OnRequest,
      this,
      _1,
      callback);

  auto request = type::UrlRequest::New();
  request->url = GetUrl();
  request->headers = headers;
  request->method = type::UrlMethod::POST;
  ledger_->LoadURL(std::move(request), url_callback);
}

void PostWalletHuhi::OnRequest(
    const type::UrlResponse& response,
    PostWalletHuhiCallback callback) {
  ledger::LogUrlResponse(__func__, response);

  std::string payment_id;
  type::Result result = CheckStatusCode(response.status_code);

  if (result != type::Result::LEDGER_OK) {
    callback(result, payment_id);
    return;
  }

  result = ParseBody(response.body, &payment_id);
  callback(result, payment_id);
}

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger
