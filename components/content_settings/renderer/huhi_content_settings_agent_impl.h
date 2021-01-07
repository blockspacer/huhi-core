/* Copyright 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_COMPONENTS_CONTENT_SETTINGS_RENDERER_HUHI_CONTENT_SETTINGS_AGENT_IMPL_H_
#define HUHI_COMPONENTS_CONTENT_SETTINGS_RENDERER_HUHI_CONTENT_SETTINGS_AGENT_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "huhi/third_party/blink/renderer/huhi_farbling_constants.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"

namespace blink {
class WebLocalFrame;
}

namespace content_settings {

// Handles blocking content per content settings for each RenderFrame.
class HuhiContentSettingsAgentImpl : public ContentSettingsAgentImpl {
 public:
  HuhiContentSettingsAgentImpl(content::RenderFrame* render_frame,
                                bool should_whitelist,
                                std::unique_ptr<Delegate> delegate);
  ~HuhiContentSettingsAgentImpl() override;

 protected:
  bool AllowScript(bool enabled_per_settings) override;
  bool AllowScriptFromSource(bool enabled_per_settings,
                             const blink::WebURL& script_url) override;
  void DidNotAllowScript() override;

  void HuhiSpecificDidBlockJavaScript(const base::string16& details);

  bool AllowAutoplay(bool default_value) override;

  bool AllowFingerprinting(bool enabled_per_settings) override;
  void DidBlockFingerprinting(const base::string16& details);

  HuhiFarblingLevel GetHuhiFarblingLevel() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(HuhiContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayBlockedByDefault);
  FRIEND_TEST_ALL_PREFIXES(HuhiContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayAllowedByDefault);

  bool IsHuhiShieldsDown(
      const blink::WebFrame* frame,
      const GURL& secondary_url);

  // RenderFrameObserver
  bool OnMessageReceived(const IPC::Message& message) override;
  void OnAllowScriptsOnce(const std::vector<std::string>& origins);
  void DidCommitProvisionalLoad(ui::PageTransition transition) override;

  bool IsScriptTemporilyAllowed(const GURL& script_url);

  // Origins of scripts which are temporary allowed for this frame in the
  // current load
  base::flat_set<std::string> temporarily_allowed_scripts_;

  // cache blocked script url which will later be used in `DidNotAllowScript()`
  GURL blocked_script_url_;

  // temporary allowed script origins we preloaded for the next load
  base::flat_set<std::string> preloaded_temporarily_allowed_scripts_;

  DISALLOW_COPY_AND_ASSIGN(HuhiContentSettingsAgentImpl);
};

}  // namespace content_settings

#endif  // HUHI_COMPONENTS_CONTENT_SETTINGS_RENDERER_HUHI_CONTENT_SETTINGS_AGENT_IMPL_H_
