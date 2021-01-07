/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 3.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/android/huhi_cosmetic_resources_tab_helper.h"

#include <memory>
#include <string>
#include <utility>

#include "huhi/browser/huhi_browser_process_impl.h"
#include "huhi/components/huhi_shields/browser/ad_block_custom_filters_service.h"
#include "huhi/components/huhi_shields/browser/ad_block_regional_service_manager.h"
#include "huhi/components/huhi_shields/browser/ad_block_service.h"
#include "huhi/components/huhi_shields/browser/ad_block_service_helper.h"
#include "huhi/components/huhi_shields/browser/huhi_shields_util.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/browser/global_routing_id.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/renderer/render_frame.h"


namespace {
bool ShouldDoCosmeticFiltering(content::WebContents* contents,
    const GURL& url) {
  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile);

  return ::huhi_shields::ShouldDoCosmeticFiltering(map, url);
}

std::unique_ptr<base::ListValue> GetUrlCosmeticResourcesOnTaskRunner(
    const std::string& url) {
  auto result_list = std::make_unique<base::ListValue>();

  base::Optional<base::Value> resources = g_huhi_browser_process->
      ad_block_service()->UrlCosmeticResources(url);

  if (!resources || !resources->is_dict()) {
    return result_list;
  }

  base::Optional<base::Value> regional_resources = g_huhi_browser_process->
      ad_block_regional_service_manager()->UrlCosmeticResources(url);

  if (regional_resources && regional_resources->is_dict()) {
    ::huhi_shields::MergeResourcesInto(
        std::move(*regional_resources), &*resources, /*force_hide=*/false);
  }

  base::Optional<base::Value> custom_resources = g_huhi_browser_process->
      ad_block_custom_filters_service()->UrlCosmeticResources(url);

  if (custom_resources && custom_resources->is_dict()) {
    ::huhi_shields::MergeResourcesInto(
        std::move(*custom_resources), &*resources, /*force_hide=*/true);
  }

  result_list->Append(std::move(*resources));

  return result_list;
}

void GetUrlCosmeticResourcesOnUI(content::GlobalFrameRoutingId frame_id,
  std::unique_ptr<base::ListValue> resources) {
  if (!resources) {
    return;
  }
  for (auto i = resources->GetList().begin();
      i < resources->GetList().end(); i++) {
    base::DictionaryValue* resources_dict;
    if (!i->GetAsDictionary(&resources_dict)) {
      continue;
    }
    std::string to_inject;
    resources_dict->GetString("injected_script", &to_inject);
    if (to_inject.length() > 1) {
      auto* frame_host = content::RenderFrameHost::FromID(frame_id);
      if (!frame_host)
        return;
      frame_host->ExecuteJavaScriptInIsolatedWorld(
          base::UTF8ToUTF16(to_inject),
          base::NullCallback(), ISOLATED_WORLD_ID_CHROME_INTERNAL);
    }
  }
}
}  // namespace


HuhiCosmeticResourcesTabHelper::HuhiCosmeticResourcesTabHelper(
    content::WebContents* contents)
    : WebContentsObserver(contents) {
}

HuhiCosmeticResourcesTabHelper::~HuhiCosmeticResourcesTabHelper() {
}

void HuhiCosmeticResourcesTabHelper::ProcessURL(
    content::WebContents* contents,
    content::RenderFrameHost* render_frame_host, const GURL& url) {
  if (!render_frame_host || !ShouldDoCosmeticFiltering(contents, url)) {
    return;
  }
  g_huhi_browser_process->ad_block_service()->GetTaskRunner()->
      PostTaskAndReplyWithResult(FROM_HERE,
          base::BindOnce(&GetUrlCosmeticResourcesOnTaskRunner, url.spec()),
          base::BindOnce(&GetUrlCosmeticResourcesOnUI,
              content::GlobalFrameRoutingId(
                  render_frame_host->GetProcess()->GetID(),
                  render_frame_host->GetRoutingID())));
}

void HuhiCosmeticResourcesTabHelper::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  ProcessURL(web_contents(), web_contents()->GetMainFrame(),
      web_contents()->GetLastCommittedURL());
}

void HuhiCosmeticResourcesTabHelper::ResourceLoadComplete(
    content::RenderFrameHost* render_frame_host,
    const content::GlobalRequestID& request_id,
    const blink::mojom::ResourceLoadInfo& resource_load_info) {
  ProcessURL(web_contents(), render_frame_host, resource_load_info.final_url);
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(HuhiCosmeticResourcesTabHelper)
