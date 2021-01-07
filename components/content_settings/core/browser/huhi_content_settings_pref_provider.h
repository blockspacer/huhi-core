/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HUHI_CONTENT_SETTINGS_PREF_PROVIDER_H_
#define HUHI_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HUHI_CONTENT_SETTINGS_PREF_PROVIDER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "components/content_settings/core/browser/content_settings_observer.h"
#include "components/content_settings/core/browser/content_settings_pref_provider.h"
#include "components/prefs/pref_change_registrar.h"

namespace content_settings {

// With this subclass, shields configuration is persisted across sessions.
// Its content type is |ContentSettingsType::PLUGIN| and its storage option is
// ephemeral because chromium want that flash configuration shouldn't be
// persisted. (Maybe chromium assumes flash is the only one of this type).
// Because of this reasion, shields configuration was also ephemeral.
// However, we want shilelds configuration persisted. To do this, we make
// EphemeralProvider ignore shields type and this class handles.
class HuhiPrefProvider : public PrefProvider,
                          public Observer {
 public:
  HuhiPrefProvider(PrefService* prefs,
                    bool off_the_record,
                    bool store_last_modified,
                    bool restore_session);
  ~HuhiPrefProvider() override;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // content_settings::PrefProvider overrides:
  void ShutdownOnUIThread() override;
  bool SetWebsiteSetting(const ContentSettingsPattern& primary_pattern,
                         const ContentSettingsPattern& secondary_pattern,
                         ContentSettingsType content_type,
                         const ResourceIdentifier& resource_identifier,
                         std::unique_ptr<base::Value>&& value,
                         const ContentSettingConstraints& constraints) override;
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      bool incognito) const override;

 private:
  friend class HuhiPrefProviderTest;
  FRIEND_TEST_ALL_PREFIXES(HuhiPrefProviderTest, TestShieldsSettingsMigration);
  FRIEND_TEST_ALL_PREFIXES(HuhiPrefProviderTest,
                           TestShieldsSettingsMigrationVersion);
  void MigrateShieldsSettings(bool incognito);
  void MigrateShieldsSettingsV1ToV2();
  void MigrateShieldsSettingsV1ToV2ForOneType(ContentSettingsType content_type,
                                              const std::string& resource_id);
  void UpdateCookieRules(ContentSettingsType content_type, bool incognito);
  void OnCookieSettingsChanged(ContentSettingsType content_type);
  void NotifyChanges(const std::vector<Rule>& rules, bool incognito);

  // content_settings::Observer overrides:
  void OnContentSettingChanged(const ContentSettingsPattern& primary_pattern,
                               const ContentSettingsPattern& secondary_pattern,
                               ContentSettingsType content_type,
                               const std::string& resource_identifier) override;
  void OnCookiePrefsChanged(const std::string& pref);

  // PrefProvider::pref_change_registrar_ alreay has plugin type.
  PrefChangeRegistrar huhi_pref_change_registrar_;

  std::map<bool /* is_incognito */, std::vector<Rule>> cookie_rules_;
  std::map<bool /* is_incognito */, std::vector<Rule>> huhi_cookie_rules_;

  bool initialized_;

  base::WeakPtrFactory<HuhiPrefProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(HuhiPrefProvider);
};

}  //  namespace content_settings

#endif  // HUHI_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HUHI_CONTENT_SETTINGS_PREF_PROVIDER_H_
