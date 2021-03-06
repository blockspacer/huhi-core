/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "huhi/browser/profiles/profile_util.h"
#include "huhi/browser/tor/buildflags.h"

#if BUILDFLAG(ENABLE_TOR)
#include "net/proxy_resolution/proxy_config_service.h"
#include "huhi/browser/tor/tor_profile_service.h"
#include "huhi/browser/tor/tor_profile_service_factory.h"
#endif

namespace {

#if BUILDFLAG(ENABLE_TOR)
std::unique_ptr<net::ProxyConfigService> CreateProxyConfigServiceTor(
    Profile* profile) {
  auto* tor_service = TorProfileServiceFactory::GetForProfile(profile);
  DCHECK(tor_service);
  return tor_service->CreateProxyConfigService();
}
#endif  // BUILDFLAG(ENABLE_TOR)

}  // namespace

#if BUILDFLAG(ENABLE_TOR)
#define HUHI_PROXY_CONFIG_MONITOR \
  if (profile && huhi::IsTorProfile(profile)) \
    proxy_config_service_ = CreateProxyConfigServiceTor(profile); \
  else
#else
#define HUHI_PROXY_CONFIG_MONITOR
#endif

#include "../../../../../chrome/browser/net/proxy_config_monitor.cc"  // NOLINT
