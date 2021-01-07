/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/tor/tor_profile_service_factory.h"

#include <memory>
#include <set>

#include "base/path_service.h"
#include "huhi/browser/huhi_browser_process_impl.h"
#include "huhi/browser/profiles/profile_util.h"
#include "huhi/components/tor/pref_names.h"
#include "huhi/components/tor/tor_profile_service_impl.h"
#include "chrome/common/chrome_paths.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_context.h"

namespace {
std::set<content::BrowserContext*> g_context_set;
}

// static
tor::TorProfileService* TorProfileServiceFactory::GetForContext(
    content::BrowserContext* context) {
  return GetForContext(context, true);
}

// static
tor::TorProfileService* TorProfileServiceFactory::GetForContext(
    content::BrowserContext* context,
    bool create) {
  return static_cast<tor::TorProfileService*>(
      GetInstance()->GetServiceForBrowserContext(context, create));
}

// static
TorProfileServiceFactory* TorProfileServiceFactory::GetInstance() {
  return base::Singleton<TorProfileServiceFactory>::get();
}

// static
void TorProfileServiceFactory::SetTorDisabled(bool disabled) {
  if (g_huhi_browser_process)
    g_huhi_browser_process->local_state()->SetBoolean(tor::prefs::kTorDisabled,
                                                       disabled);
}

// static
bool TorProfileServiceFactory::IsTorDisabled() {
  if (g_huhi_browser_process)
    return g_huhi_browser_process->local_state()->GetBoolean(
        tor::prefs::kTorDisabled);
  return false;
}

TorProfileServiceFactory::TorProfileServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "TorProfileService",
          BrowserContextDependencyManager::GetInstance()) {
  g_context_set.clear();
}

TorProfileServiceFactory::~TorProfileServiceFactory() {}

KeyedService* TorProfileServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  if (!huhi::IsTorProfile(context))
    return nullptr;
  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  DCHECK(!user_data_dir.empty());
  std::unique_ptr<tor::TorProfileService> tor_profile_service(
      new tor::TorProfileServiceImpl(
          context,
          g_huhi_browser_process
              ? g_huhi_browser_process->tor_client_updater()
              : nullptr,
          user_data_dir));

  // We only care about Tor incognito profiles for deciding whether to KillTor.
  if (context->IsOffTheRecord()) {
    g_context_set.emplace(context);
  }

  return tor_profile_service.release();
}

content::BrowserContext* TorProfileServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  // Not shared with our dummy regular Tor profile because we want to trigger
  // LaunchTor when a new Tor window is created.
  return context;
}

void TorProfileServiceFactory::BrowserContextShutdown(
    content::BrowserContext* context) {
  // KillTor when the last Tor incognito profile is shutting down.
  if (g_context_set.size() == 1) {
    auto* service = static_cast<tor::TorProfileServiceImpl*>(
        TorProfileServiceFactory::GetForContext(context, false));
    if (service) {
      service->KillTor();
    } else {
      DCHECK(!huhi::IsTorProfile(context));
    }
  }
  BrowserContextKeyedServiceFactory::BrowserContextShutdown(context);
}

void TorProfileServiceFactory::BrowserContextDestroyed(
    content::BrowserContext* context) {
  g_context_set.erase(context);
  BrowserContextKeyedServiceFactory::BrowserContextDestroyed(context);
}
