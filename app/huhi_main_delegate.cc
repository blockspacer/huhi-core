/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/app/huhi_main_delegate.h"

#include <memory>
#include <string>
#include <unordered_set>

#include "base/base_switches.h"
#include "base/lazy_instance.h"
#include "base/path_service.h"
#include "base/task/post_task.h"
#include "base/time/time.h"
#include "huhi/app/huhi_command_line_helper.h"
#include "huhi/browser/huhi_content_browser_client.h"
#include "huhi/common/huhi_switches.h"
#include "huhi/common/resource_bundle_helper.h"
#include "huhi/components/huhi_ads/browser/buildflags/buildflags.h"
#include "huhi/renderer/huhi_content_renderer_client.h"
#include "huhi/utility/huhi_content_utility_client.h"
#include "build/build_config.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/autofill/core/common/autofill_features.h"
#include "components/autofill/core/common/autofill_payments_features.h"
#include "components/embedder_support/switches.h"
#include "components/feed/feed_feature_list.h"
#include "components/language/core/common/language_experiments.h"
#include "components/offline_pages/core/offline_page_feature.h"
#include "components/omnibox/common/omnibox_features.h"
#include "components/password_manager/core/common/password_manager_features.h"
#include "components/safe_browsing/core/features.h"
#include "components/security_state/core/features.h"
#include "components/sync/base/sync_base_switches.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "content/public/common/content_features.h"
#include "content/public/common/content_switches.h"
#include "google_apis/gaia/gaia_switches.h"
#include "net/base/features.h"
#include "services/device/public/cpp/device_features.h"
#include "services/network/public/cpp/features.h"
#include "third_party/blink/public/common/features.h"
#include "third_party/widevine/cdm/buildflags.h"
#include "ui/base/ui_base_features.h"

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
#include "huhi/common/huhi_paths.h"
#endif

#if BUILDFLAG(HUHI_ADS_ENABLED)
#include "components/dom_distiller/core/dom_distiller_switches.h"
#endif

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#include "huhi/build/android/jni_headers/HuhiQAPreferences_jni.h"
#endif

namespace {
// staging "https://sync-v2.huhisoft.com/v2" can be overriden by
// switches::kSyncServiceURL manually
const char kHuhiSyncServiceStagingURL[] =
    "https://sync-v2.huhisoft.com/v2";
#if defined(OFFICIAL_BUILD)
// production
const char kHuhiSyncServiceURL[] = "https://sync-v2.huhisoft.com/v2";
#else
// For local server development "http://localhost:8295/v2 can also be overriden
// by switches::kSyncServiceURL
// dev
const char kHuhiSyncServiceURL[] = "https://sync-v2.huhi.software/v2";
#endif
}  // namespace

#if !defined(CHROME_MULTIPLE_DLL_BROWSER)
base::LazyInstance<HuhiContentRendererClient>::DestructorAtExit
    g_huhi_content_renderer_client = LAZY_INSTANCE_INITIALIZER;
base::LazyInstance<HuhiContentUtilityClient>::DestructorAtExit
    g_huhi_content_utility_client = LAZY_INSTANCE_INITIALIZER;
#endif
#if !defined(CHROME_MULTIPLE_DLL_CHILD)
base::LazyInstance<HuhiContentBrowserClient>::DestructorAtExit
    g_huhi_content_browser_client = LAZY_INSTANCE_INITIALIZER;
#endif

const char kHuhiOriginTrialsPublicKey[] =
    "bYUKPJoPnCxeNvu72j4EmPuK7tr1PAC7SHh8ld9Mw3E=,"
    "fMS4mpO6buLQ/QMd+zJmxzty/VQ6B1EUZqoCU04zoRU=";

const char kDummyUrl[] = "https://no-thanks.invalid";

HuhiMainDelegate::HuhiMainDelegate()
    : ChromeMainDelegate() {}

HuhiMainDelegate::HuhiMainDelegate(base::TimeTicks exe_entry_point_ticks)
    : ChromeMainDelegate(exe_entry_point_ticks) {}

HuhiMainDelegate::~HuhiMainDelegate() {}

content::ContentBrowserClient*
HuhiMainDelegate::CreateContentBrowserClient() {
#if defined(CHROME_MULTIPLE_DLL_CHILD)
  return NULL;
#else
  if (chrome_content_browser_client_ == nullptr) {
    DCHECK(!startup_data_);
    startup_data_ = std::make_unique<StartupData>();
    chrome_content_browser_client_ =
        std::make_unique<HuhiContentBrowserClient>(startup_data_.get());
  }
  return chrome_content_browser_client_.get();
#endif
}

content::ContentRendererClient*
HuhiMainDelegate::CreateContentRendererClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_huhi_content_renderer_client.Pointer();
#endif
}

content::ContentUtilityClient*
HuhiMainDelegate::CreateContentUtilityClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_huhi_content_utility_client.Pointer();
#endif
}

void HuhiMainDelegate::PreSandboxStartup() {
  ChromeMainDelegate::PreSandboxStartup();
#if defined(OS_LINUX) || defined(OS_MAC)
  // Setup NativeMessagingHosts to point to the default Chrome locations
  // because that's where native apps will create them
  base::FilePath chrome_user_data_dir;
  base::FilePath native_messaging_dir;
#if defined(OS_MAC)
  base::PathService::Get(base::DIR_APP_DATA, &chrome_user_data_dir);
  chrome_user_data_dir = chrome_user_data_dir.Append("Google/Chrome");
  native_messaging_dir = base::FilePath(FILE_PATH_LITERAL(
      "/Library/Google/Chrome/NativeMessagingHosts"));
#else
  chrome::GetDefaultUserDataDirectory(&chrome_user_data_dir);
  native_messaging_dir = base::FilePath(FILE_PATH_LITERAL(
      "/etc/opt/chrome/native-messaging-hosts"));
#endif  // defined(OS_MAC)
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_USER_NATIVE_MESSAGING,
      chrome_user_data_dir.Append(FILE_PATH_LITERAL("NativeMessagingHosts")),
      false, true);
  base::PathService::OverrideAndCreateIfNeeded(chrome::DIR_NATIVE_MESSAGING,
      native_messaging_dir, false, true);
#endif  // defined(OS_LINUX) || defined(OS_MAC)
  if (huhi::SubprocessNeedsResourceBundle()) {
    huhi::InitializeResourceBundle();
  }
}

bool HuhiMainDelegate::BasicStartupComplete(int* exit_code) {
  HuhiCommandLineHelper command_line(base::CommandLine::ForCurrentProcess());
#if BUILDFLAG(HUHI_ADS_ENABLED)
  command_line.AppendSwitch(switches::kEnableDomDistiller);
#endif
  command_line.AppendSwitch(switches::kDisableDomainReliability);
  command_line.AppendSwitch(switches::kNoPings);

  // Setting these to default values in Chromium to maintain parity
  // See: ChromeContentVerifierDelegate::GetDefaultMode for ContentVerification
  // See: GetStatus in install_verifier.cc for InstallVerification
  command_line.AppendSwitchASCII(switches::kExtensionContentVerification,
      switches::kExtensionContentVerificationEnforceStrict);
  command_line.AppendSwitchASCII(switches::kExtensionsInstallVerification,
      "enforce");

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          embedder_support::kOriginTrialPublicKey)) {
    command_line.AppendSwitchASCII(embedder_support::kOriginTrialPublicKey,
                                   kHuhiOriginTrialsPublicKey);
  }

  std::string huhi_sync_service_url = kHuhiSyncServiceURL;
#if defined(OS_ANDROID)
  AdjustSyncServiceUrlForAndroid(&huhi_sync_service_url);
#endif  // defined(OS_ANDROID)

  // Huhi's sync protocol does not use the sync service url
  command_line.AppendSwitchASCII(switches::kSyncServiceURL,
                                 huhi_sync_service_url.c_str());

  command_line.AppendSwitchASCII(switches::kLsoUrl, kDummyUrl);

  // Enabled features.
  std::unordered_set<const char*> enabled_features = {
      // Upgrade all mixed content
      blink::features::kMixedContentAutoupgrade.name,
      password_manager::features::kPasswordImport.name,
      net::features::kLegacyTLSEnforced.name,
      // Remove URL bar mixed control and allow site specific override instead
      features::kMixedContentSiteSetting.name,
      // Warn about Mixed Content optionally blockable content
      security_state::features::kPassiveMixedContentWarning.name,
      // Enable webui dark theme: @media (prefers-color-scheme: dark) is gated
      // on
      // this feature.
      features::kWebUIDarkMode.name,
      blink::features::kPrefetchPrivacyChanges.name,
      blink::features::kReducedReferrerGranularity.name,
#if defined(OS_WIN)
      features::kWinrtGeolocationImplementation.name,
#endif
      omnibox::kOmniboxContextMenuShowFullUrls.name,
  };

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableDnsOverHttps)) {
    enabled_features.insert(features::kDnsOverHttps.name);
  }

  // Disabled features.
  const std::unordered_set<const char*> disabled_features = {
    autofill::features::kAutofillEnableAccountWalletStorage.name,
    autofill::features::kAutofillServerCommunication.name,
    blink::features::kTextFragmentAnchor.name,
    features::kAllowPopupsDuringPageUnload.name,
    features::kNotificationTriggers.name,
    features::kPrivacySettingsRedesign.name,
    features::kSmsReceiver.name,
    features::kVideoPlaybackQuality.name,
    features::kTabHoverCards.name,
    password_manager::features::kPasswordCheck.name,
    safe_browsing::kEnhancedProtection.name,
#if defined(OS_ANDROID)
    feed::kInterestFeedContentSuggestions.name,
    translate::kTranslateUI.name,
    offline_pages::kPrefetchingOfflinePagesFeature.name,
#endif
  };
  command_line.AppendFeatures(enabled_features, disabled_features);

  bool ret = ChromeMainDelegate::BasicStartupComplete(exit_code);

#if BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  // Override chrome::DIR_BUNDLED_WIDEVINE_CDM path because we install it in
  // user data dir. Must call after ChromeMainDelegate::BasicStartupComplete()
  // to use chrome paths.
  huhi::OverridePath();
#endif

  return ret;
}

#if defined(OS_ANDROID)
void HuhiMainDelegate::AdjustSyncServiceUrlForAndroid(
    std::string* huhi_sync_service_url) {
  DCHECK_NE(huhi_sync_service_url, nullptr);
  const char kProcessTypeSwitchName[] = "type";

  // On Android we can detect data dir only on host process, and we cannot
  // for example on renderer or gpu-process, because JNI is not initialized
  // And no sense to override sync service url for them in anyway
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          kProcessTypeSwitchName)) {
    // This is something other than browser process
    return;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  bool b_use_staging_sync_server =
      Java_HuhiQAPreferences_isSyncStagingUsed(env);
  if (b_use_staging_sync_server) {
    *huhi_sync_service_url = kHuhiSyncServiceStagingURL;
  }
}
#endif  // defined(OS_ANDROID)