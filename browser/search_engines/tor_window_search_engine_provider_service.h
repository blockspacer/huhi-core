/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
#define HUHI_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_

#include <memory>
#include "huhi/browser/search_engines/search_engine_provider_service.h"
#include "components/prefs/pref_member.h"
#include "components/search_engines/template_url_service_observer.h"

class PrefService;
struct TemplateURLData;

// The purpose of this service for tor is making user changed search engine
// provider persist across the sessions.
// Also, HuhiProfileManager::SetNonPersonalProfilePrefs() overrides for it.
class TorWindowSearchEngineProviderService
    : public SearchEngineProviderService,
      public TemplateURLServiceObserver {
 public:
  explicit TorWindowSearchEngineProviderService(Profile* otr_profile);
  ~TorWindowSearchEngineProviderService() override;

 private:
  // TemplateURLServiceObserver overrides:
  void OnTemplateURLServiceChanged() override;

  std::unique_ptr<TemplateURLData> GetInitialSearchEngineProvider(
      PrefService* prefs) const;

  IntegerPrefMember alternative_search_engine_provider_in_tor_;

  DISALLOW_COPY_AND_ASSIGN(TorWindowSearchEngineProviderService);
};

#endif  // HUHI_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
