/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_NET_HUHI_REQUEST_HANDLER_H_
#define HUHI_BROWSER_NET_HUHI_REQUEST_HANDLER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "huhi/browser/net/url_context.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/completion_once_callback.h"

class PrefChangeRegistrar;

// Contains different network stack hooks (similar to capabilities of WebRequest
// API).
class HuhiRequestHandler {
 public:
  using ResponseCallback = base::Callback<void(const base::DictionaryValue&)>;

  HuhiRequestHandler();
  ~HuhiRequestHandler();

  bool IsRequestIdentifierValid(uint64_t request_identifier);

  int OnBeforeURLRequest(std::shared_ptr<huhi::HuhiRequestInfo> ctx,
                         net::CompletionOnceCallback callback,
                         GURL* new_url);

  int OnBeforeStartTransaction(std::shared_ptr<huhi::HuhiRequestInfo> ctx,
                               net::CompletionOnceCallback callback,
                               net::HttpRequestHeaders* headers);
  int OnHeadersReceived(
      std::shared_ptr<huhi::HuhiRequestInfo> ctx,
      net::CompletionOnceCallback callback,
      const net::HttpResponseHeaders* original_response_headers,
      scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url);

  void OnURLRequestDestroyed(std::shared_ptr<huhi::HuhiRequestInfo> ctx);
  void RunCallbackForRequestIdentifier(uint64_t request_identifier, int rv);

 private:
  void SetupCallbacks();
  void InitPrefChangeRegistrar();
  void OnReferralHeadersChanged();
  void OnPreferenceChanged(const std::string& pref_name);
  void UpdateAdBlockFromPref(const std::string& pref_name);

  void RunNextCallback(std::shared_ptr<huhi::HuhiRequestInfo> ctx);

  std::vector<huhi::OnBeforeURLRequestCallback> before_url_request_callbacks_;
  std::vector<huhi::OnBeforeStartTransactionCallback>
      before_start_transaction_callbacks_;
  std::vector<huhi::OnHeadersReceivedCallback> headers_received_callbacks_;

  // TODO(iefremov): actually, we don't have to keep the list here, since
  // it is global for the whole browser and could live a singletonce in the
  // rewards service. Eliminating this will also help to avoid using
  // PrefChangeRegistrar and corresponding |base::Unretained| usages, that are
  // illegal.
  std::unique_ptr<base::ListValue> referral_headers_list_;
  std::map<uint64_t, net::CompletionOnceCallback> callbacks_;
  std::unique_ptr<PrefChangeRegistrar, content::BrowserThread::DeleteOnUIThread>
      pref_change_registrar_;

  base::WeakPtrFactory<HuhiRequestHandler> weak_factory_{this};
  DISALLOW_COPY_AND_ASSIGN(HuhiRequestHandler);
};

#endif  // HUHI_BROWSER_NET_HUHI_REQUEST_HANDLER_H_
