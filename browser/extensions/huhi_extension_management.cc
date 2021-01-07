/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/extensions/huhi_extension_management.h"

#include <memory>

#include "huhi/browser/huhi_browser_process_impl.h"
#include "huhi/browser/extensions/huhi_extension_provider.h"
#include "huhi/common/pref_names.h"
#include "huhi/browser/tor/tor_profile_service_factory.h"
#include "huhi/components/ipfs/buildflags/buildflags.h"
#include "huhi/components/tor/buildflags/buildflags.h"
#include "huhi/components/tor/pref_names.h"
#include "chrome/browser/extensions/external_policy_loader.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_urls.h"

#if BUILDFLAG(ENABLE_TOR)
#include "huhi/components/tor/huhi_tor_client_updater.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "huhi/browser/ipfs/ipfs_service_factory.h"
#include "huhi/components/ipfs/huhi_ipfs_client_updater.h"
#endif

namespace extensions {

HuhiExtensionManagement::HuhiExtensionManagement(Profile* profile)
    : ExtensionManagement(profile),
      extension_registry_observer_(this) {
  extension_registry_observer_.Add(ExtensionRegistry::Get(
        static_cast<content::BrowserContext*>(profile)));
  providers_.push_back(
      std::make_unique<HuhiExtensionProvider>());
  local_state_pref_change_registrar_.Init(g_browser_process->local_state());
  local_state_pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::BindRepeating(&HuhiExtensionManagement::OnTorDisabledChanged,
                          base::Unretained(this)));
  Cleanup();
}

HuhiExtensionManagement::~HuhiExtensionManagement() {
  local_state_pref_change_registrar_.RemoveAll();
}

void HuhiExtensionManagement::OnExtensionLoaded(
    content::BrowserContext* browser_context,
    const Extension* extension) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, true);
}

void HuhiExtensionManagement::OnExtensionUnloaded(
    content::BrowserContext* browser_context,
    const Extension* extension,
    UnloadedExtensionReason reason) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, false);
}

void HuhiExtensionManagement::OnTorDisabledChanged() {
#if BUILDFLAG(ENABLE_TOR)
  if (TorProfileServiceFactory::IsTorDisabled())
    g_huhi_browser_process->tor_client_updater()->Cleanup();
#endif
}

void HuhiExtensionManagement::Cleanup() {
  // BrowserPolicyConnector enforce policy earlier than this constructor so we
  // have to manully cleanup tor executable when tor is disabled by gpo
  OnTorDisabledChanged();

#if BUILDFLAG(IPFS_ENABLED)
  // Remove ipfs executable if it is disabled by GPO.
  if (ipfs::IpfsServiceFactory::IsIpfsDisabledByPolicy())
    g_huhi_browser_process->ipfs_client_updater()->Cleanup();
#endif
}

}  // namespace extensions
