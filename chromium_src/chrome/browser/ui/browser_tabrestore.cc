/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/huhi_webtorrent/browser/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_tabrestore.h"

#if BUILDFLAG(ENABLE_HUHI_WEBTORRENT)
#include "huhi/browser/extensions/huhi_component_loader.h"
#include "huhi/browser/extensions/huhi_webtorrent_navigation_throttle.h"
#include "huhi/components/huhi_webtorrent/browser/webtorrent_util.h"
#include "chrome/browser/extensions/extension_service.h"
#include "extensions/browser/extension_system.h"
#endif

#define AddRestoredTab AddRestoredTab_ChromiumImpl
#include "../../../../../chrome/browser/ui/browser_tabrestore.cc"
#undef AddRestoredTab

namespace {

#if BUILDFLAG(ENABLE_HUHI_WEBTORRENT)
void MaybeLoadWebtorrent(Browser* browser,
                         bool from_session_restore,
                         const GURL& restore_url) {
  if (!from_session_restore || !webtorrent::IsWebtorrentURL(restore_url))
    return;

  extensions::HuhiWebTorrentNavigationThrottle::MaybeLoadWebtorrent(
      browser->profile(), restore_url);
}
#endif

}  // namespace

namespace chrome {

WebContents* AddRestoredTab(
    Browser* browser,
    const std::vector<SerializedNavigationEntry>& navigations,
    int tab_index,
    int selected_navigation,
    const std::string& extension_app_id,
    base::Optional<tab_groups::TabGroupId> group,
    bool select,
    bool pin,
    bool from_last_session,
    base::TimeTicks last_active_time,
    content::SessionStorageNamespace* session_storage_namespace,
    const sessions::SerializedUserAgentOverride& user_agent_override,
    bool from_session_restore) {
#if BUILDFLAG(ENABLE_HUHI_WEBTORRENT)
  MaybeLoadWebtorrent(
      browser,
      from_session_restore,
      navigations.at(selected_navigation).original_request_url());
#endif

  return AddRestoredTab_ChromiumImpl(browser, navigations, tab_index,
      selected_navigation, extension_app_id, group, select, pin,
      from_last_session, last_active_time, session_storage_namespace,
      user_agent_override, from_session_restore);
}

}  // namespace chrome