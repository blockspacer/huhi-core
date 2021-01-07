/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_GEMINI_GEMINI_SERVICE_FACTORY_H_
#define HUHI_BROWSER_GEMINI_GEMINI_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class GeminiService;
class Profile;

class GeminiServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static GeminiService* GetForProfile(Profile* profile);
  static GeminiServiceFactory* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<GeminiServiceFactory>;

  GeminiServiceFactory();
  ~GeminiServiceFactory() override;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  DISALLOW_COPY_AND_ASSIGN(GeminiServiceFactory);
};

#endif  // HUHI_BROWSER_GEMINI_GEMINI_SERVICE_FACTORY_H_
