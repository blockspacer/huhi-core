// Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Huhi Software
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "huhi/browser/ui/webui/huhi_new_tab_message_handler.h"

#include <string>
#include <memory>
#include <utility>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "huhi/browser/profiles/profile_util.h"
#include "huhi/browser/search_engines/search_engine_provider_util.h"
#include "huhi/browser/ui/webui/huhi_new_tab_ui.h"
#include "huhi/browser/ntp_background_images/view_counter_service_factory.h"
#include "huhi/common/pref_names.h"
#include "huhi/components/huhi_ads/browser/ads_service.h"
#include "huhi/components/huhi_ads/browser/ads_service_factory.h"
#include "huhi/components/huhi_perf_predictor/browser/buildflags.h"
#include "huhi/components/moonpay/browser/buildflags/buildflags.h"
#include "huhi/components/ntp_background_images/browser/features.h"
#include "huhi/components/ntp_background_images/browser/view_counter_service.h"
#include "huhi/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_features.h"
#include "content/public/browser/web_ui_data_source.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"

using ntp_background_images::features::kHuhiNTPBrandedWallpaper;
using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::kNewTabPageShowSponsoredImagesBackgroundImage;  // NOLINT
using ntp_background_images::prefs::kBrandedWallpaperNotificationDismissed;
using ntp_background_images::ViewCounterServiceFactory;

#if BUILDFLAG(ENABLE_HUHI_PERF_PREDICTOR)
#include "huhi/components/huhi_perf_predictor/common/pref_names.h"
#endif

#if BUILDFLAG(MOONPAY_ENABLED)
#include "huhi/components/moonpay/common/pref_names.h"
#endif

namespace {

bool IsPrivateNewTab(Profile* profile) {
  return huhi::IsTorProfile(profile) ||
         profile->IsIncognitoProfile() ||
         profile->IsGuestSession();
}

base::DictionaryValue GetStatsDictionary(PrefService* prefs) {
  base::DictionaryValue stats_data;
  stats_data.SetInteger(
    "adsBlockedStat",
    prefs->GetUint64(kAdsBlocked) + prefs->GetUint64(kTrackersBlocked));
  stats_data.SetInteger(
    "javascriptBlockedStat",
    prefs->GetUint64(kJavascriptBlocked));
  stats_data.SetInteger(
    "fingerprintingBlockedStat",
    prefs->GetUint64(kFingerprintingBlocked));
#if BUILDFLAG(ENABLE_HUHI_PERF_PREDICTOR)
  stats_data.SetDouble(
      "bandwidthSavedStat",
      prefs->GetUint64(huhi_perf_predictor::prefs::kBandwidthSavedBytes));
#endif
  return stats_data;
}

base::DictionaryValue GetPreferencesDictionary(PrefService* prefs) {
  base::DictionaryValue pref_data;
  pref_data.SetBoolean(
      "showBackgroundImage",
      prefs->GetBoolean(kNewTabPageShowBackgroundImage));
  pref_data.SetBoolean(
      "brandedWallpaperOptIn",
      prefs->GetBoolean(kNewTabPageShowSponsoredImagesBackgroundImage));
  pref_data.SetBoolean(
      "showClock",
      prefs->GetBoolean(kNewTabPageShowClock));
  pref_data.SetBoolean(
      "showTopSites",
      prefs->GetBoolean(kNewTabPageShowTopSites));
  pref_data.SetBoolean(
      "showStats",
      prefs->GetBoolean(kNewTabPageShowStats));
  pref_data.SetBoolean(
      "showRewards",
      prefs->GetBoolean(kNewTabPageShowRewards));
  pref_data.SetBoolean(
      "isBrandedWallpaperNotificationDismissed",
      prefs->GetBoolean(kBrandedWallpaperNotificationDismissed));
  pref_data.SetBoolean(
      "showBinance",
      prefs->GetBoolean(kNewTabPageShowBinance));
  pref_data.SetBoolean(
      "showTogether",
      prefs->GetBoolean(kNewTabPageShowTogether));
  pref_data.SetBoolean(
      "showAddCard",
      prefs->GetBoolean(kNewTabPageShowAddCard));
  pref_data.SetBoolean(
      "showGemini",
      prefs->GetBoolean(kNewTabPageShowGemini));
#if BUILDFLAG(MOONPAY_ENABLED)
  pref_data.SetBoolean(
      "showBitcoinDotCom",
      prefs->GetBoolean(kMoonpayNewTabPageShowBitcoinDotCom));
#endif
  return pref_data;
}

base::DictionaryValue GetPrivatePropertiesDictionary(PrefService* prefs) {
  base::DictionaryValue private_data;
  private_data.SetBoolean(
      "useAlternativePrivateSearchEngine",
      prefs->GetBoolean(kUseAlternativeSearchEngineProvider));
  return private_data;
}

}  // namespace

// static
HuhiNewTabMessageHandler* HuhiNewTabMessageHandler::Create(
      content::WebUIDataSource* source, Profile* profile) {
  //
  // Initial Values
  // Should only contain data that is static
  //
  auto* ads_service_ = huhi_ads::AdsServiceFactory::GetForProfile(profile);
  // For safety, default |is_ads_supported_locale_| to true. Better to have
  // false positive than falsen egative,
  // in which case we would not show "opt out" toggle.
  bool is_ads_supported_locale_ = true;
  if (!ads_service_) {
    LOG(ERROR) << "Ads service is not initialized!";
  } else {
    is_ads_supported_locale_ = ads_service_->IsSupportedLocale();
  }

  source->AddBoolean(
      "featureFlagHuhiNTPSponsoredImagesWallpaper",
      base::FeatureList::IsEnabled(kHuhiNTPBrandedWallpaper) &&
      is_ads_supported_locale_);
  // Private Tab info
  if (IsPrivateNewTab(profile)) {
    source->AddBoolean(
      "isTor", huhi::IsTorProfile(profile));
    source->AddBoolean(
      "isQwant", huhi::IsRegionForQwant(profile));
  }
  return new HuhiNewTabMessageHandler(profile);
}

HuhiNewTabMessageHandler::HuhiNewTabMessageHandler(Profile* profile)
    : profile_(profile) {
}

HuhiNewTabMessageHandler::~HuhiNewTabMessageHandler() {}

void HuhiNewTabMessageHandler::RegisterMessages() {
  // TODO(petemill): This MessageHandler can be split up to
  // individual MessageHandlers for each individual topic area,
  // should other WebUI pages wish to consume the APIs:
  // - Stats
  // - Preferences
  // - PrivatePage properties
  web_ui()->RegisterMessageCallback(
    "getNewTabPagePreferences",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleGetPreferences,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "getNewTabPageStats",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleGetStats,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "getNewTabPagePrivateProperties",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleGetPrivateProperties,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "toggleAlternativePrivateSearchEngine",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleToggleAlternativeSearchEngineProvider,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "saveNewTabPagePref",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleSaveNewTabPagePref,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "registerNewTabPageView",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleRegisterNewTabPageView,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "getBrandedWallpaperData",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleGetBrandedWallpaperData,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
    "getDefaultSuperReferralTopSitesData",
    base::BindRepeating(
      &HuhiNewTabMessageHandler::HandleGetDefaultSuperReferralTopSitesData,
      base::Unretained(this)));
}

void HuhiNewTabMessageHandler::OnJavascriptAllowed() {
  // Observe relevant preferences
  PrefService* prefs = profile_->GetPrefs();
  pref_change_registrar_.Init(prefs);
  // Stats
  pref_change_registrar_.Add(kAdsBlocked,
    base::Bind(&HuhiNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kTrackersBlocked,
    base::Bind(&HuhiNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kJavascriptBlocked,
    base::Bind(&HuhiNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kHttpsUpgrades,
    base::Bind(&HuhiNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kFingerprintingBlocked,
    base::Bind(&HuhiNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));

  if (IsPrivateNewTab(profile_)) {
    // Private New Tab Page preferences
    pref_change_registrar_.Add(kUseAlternativeSearchEngineProvider,
      base::Bind(&HuhiNewTabMessageHandler::OnPrivatePropertiesChanged,
      base::Unretained(this)));
    pref_change_registrar_.Add(kAlternativeSearchEngineProviderInTor,
      base::Bind(&HuhiNewTabMessageHandler::OnPrivatePropertiesChanged,
      base::Unretained(this)));
  }
  // New Tab Page preferences
  pref_change_registrar_.Add(kNewTabPageShowBackgroundImage,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowSponsoredImagesBackgroundImage,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowClock,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowStats,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowTopSites,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowRewards,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kBrandedWallpaperNotificationDismissed,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowBinance,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowTogether,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowAddCard,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowGemini,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
#if BUILDFLAG(MOONPAY_ENABLED)
  pref_change_registrar_.Add(kMoonpayNewTabPageShowBitcoinDotCom,
    base::Bind(&HuhiNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
#endif
}

void HuhiNewTabMessageHandler::OnJavascriptDisallowed() {
  pref_change_registrar_.RemoveAll();
}

void HuhiNewTabMessageHandler::HandleGetPreferences(
        const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void HuhiNewTabMessageHandler::HandleGetStats(const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void HuhiNewTabMessageHandler::HandleGetPrivateProperties(
        const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void HuhiNewTabMessageHandler::HandleToggleAlternativeSearchEngineProvider(
    const base::ListValue* args) {
  huhi::ToggleUseAlternativeSearchEngineProvider(profile_);
}

void HuhiNewTabMessageHandler::HandleSaveNewTabPagePref(
    const base::ListValue* args) {
  if (args->GetSize() != 2) {
    LOG(ERROR) << "Invalid input";
    return;
  }
  PrefService* prefs = profile_->GetPrefs();
  // Collect args
  std::string settingsKeyInput = args->GetList()[0].GetString();
  auto settingsValue = args->GetList()[1].Clone();
  // Validate args
  // Note: if we introduce any non-bool settings values
  // then perform this type check within the appropriate key conditionals.
  if (!settingsValue.is_bool()) {
    LOG(ERROR) << "Invalid value type";
    return;
  }
  const auto settingsValueBool = settingsValue.GetBool();
  std::string settingsKey;
  if (settingsKeyInput == "showBackgroundImage") {
    settingsKey = kNewTabPageShowBackgroundImage;
  } else if (settingsKeyInput == "brandedWallpaperOptIn") {
    // TODO(simonhong): I think above |brandedWallpaperOptIn| should be changed
    // to |sponsoredImagesWallpaperOptIn|.
    settingsKey = kNewTabPageShowSponsoredImagesBackgroundImage;
  } else if (settingsKeyInput == "showClock") {
    settingsKey = kNewTabPageShowClock;
  } else if (settingsKeyInput == "showTopSites") {
    settingsKey = kNewTabPageShowTopSites;
  } else if (settingsKeyInput == "showStats") {
    settingsKey = kNewTabPageShowStats;
  } else if (settingsKeyInput == "showRewards") {
    settingsKey = kNewTabPageShowRewards;
  } else if (settingsKeyInput == "isBrandedWallpaperNotificationDismissed") {
    settingsKey = kBrandedWallpaperNotificationDismissed;
  } else if (settingsKeyInput == "showBinance") {
    settingsKey = kNewTabPageShowBinance;
  } else if (settingsKeyInput == "showTogether") {
    settingsKey = kNewTabPageShowTogether;
  } else if (settingsKeyInput == "showAddCard") {
    settingsKey = kNewTabPageShowAddCard;
  } else if (settingsKeyInput == "showGemini") {
    settingsKey = kNewTabPageShowGemini;
#if BUILDFLAG(MOONPAY_ENABLED)
  } else if (settingsKeyInput == "showBitcoinDotCom") {
    settingsKey = kMoonpayNewTabPageShowBitcoinDotCom;
#endif
  } else {
    LOG(ERROR) << "Invalid setting key";
    return;
  }
  prefs->SetBoolean(settingsKey, settingsValueBool);
}

void HuhiNewTabMessageHandler::HandleRegisterNewTabPageView(
    const base::ListValue* args) {
  AllowJavascript();

  // Decrement original value only if there's actual branded content
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile_))
    service->RegisterPageView();
}

void HuhiNewTabMessageHandler::HandleGetBrandedWallpaperData(
    const base::ListValue* args) {
  AllowJavascript();

  auto* service = ViewCounterServiceFactory::GetForProfile(profile_);
  ResolveJavascriptCallback(
      args->GetList()[0],
      service ? service->GetCurrentWallpaperForDisplay() : base::Value());
}

void HuhiNewTabMessageHandler::HandleGetDefaultSuperReferralTopSitesData(
    const base::ListValue* args) {
  AllowJavascript();

  auto* service = ViewCounterServiceFactory::GetForProfile(profile_);
  ResolveJavascriptCallback(
      args->GetList()[0],
      service ? service->GetTopSites(true) : base::Value());
}

void HuhiNewTabMessageHandler::OnPrivatePropertiesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  FireWebUIListener("private-tab-data-updated", data);
}

void HuhiNewTabMessageHandler::OnStatsChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  FireWebUIListener("stats-updated", data);
}

void HuhiNewTabMessageHandler::OnPreferencesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  FireWebUIListener("preferences-changed", data);
}
