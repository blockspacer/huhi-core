/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/huhi_rewards/extension_rewards_service_observer.h"

#include <utility>
#include <string>
#include <vector>

#include "base/base64.h"
#include "huhi/common/extensions/api/huhi_rewards.h"
#include "huhi/components/huhi_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"

namespace huhi_rewards {

ExtensionRewardsServiceObserver::ExtensionRewardsServiceObserver(
    Profile* profile)
    : profile_(profile) {
}

ExtensionRewardsServiceObserver::~ExtensionRewardsServiceObserver() {
}

void ExtensionRewardsServiceObserver::OnWalletInitialized(
    RewardsService* rewards_service,
    const ledger::type::Result result) {
  auto* event_router = extensions::EventRouter::Get(profile_);

  if (!event_router) {
    return;
  }

  if (result == ledger::type::Result::WALLET_CREATED) {
    auto args = std::make_unique<base::ListValue>();
    std::unique_ptr<extensions::Event> event(new extensions::Event(
        extensions::events::HUHI_START,
        extensions::api::huhi_rewards::WalletCreated::kEventName,
        std::move(args)));
    event_router->BroadcastEvent(std::move(event));
    return;
  }

  if (result != ledger::type::Result::NO_LEDGER_STATE &&
      result != ledger::type::Result::LEDGER_OK) {
    // Report back all errors except when ledger_state is missing
    std::unique_ptr<base::ListValue> args(
        extensions::api::huhi_rewards::WalletCreationFailed::Create(
          static_cast<int>(result)).release());

    std::unique_ptr<extensions::Event> event(new extensions::Event(
        extensions::events::HUHI_START,
        extensions::api::huhi_rewards::WalletCreationFailed::kEventName,
        std::move(args)));
    event_router->BroadcastEvent(std::move(event));
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::Initialized::Create(
        static_cast<int>(result)).release());

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::Initialized::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPanelPublisherInfo(
    RewardsService* rewards_service,
    const ledger::type::Result result,
    const ledger::type::PublisherInfo* info,
    uint64_t windowId) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router || !info) {
    return;
  }

  extensions::api::huhi_rewards::OnPublisherData::Publisher publisher;

  publisher.percentage = info->percent;
  publisher.status = static_cast<int>(info->status);
  publisher.excluded =
      info->excluded == ledger::type::PublisherExclude::EXCLUDED;
  publisher.name = info->name;
  publisher.url = info->url;
  publisher.provider = info->provider;
  publisher.favicon_url = info->favicon_url;
  publisher.publisher_key = info->id;
  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnPublisherData::Create(windowId,
                                                              publisher)
          .release());

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_ON_PUBLISHER_DATA,
      extensions::api::huhi_rewards::OnPublisherData::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnFetchPromotions(
    RewardsService* rewards_service,
    const ledger::type::Result result,
    const ledger::type::PromotionList& list) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::vector<extensions::api::huhi_rewards::OnPromotions::
        PromotionsType> promotions;

  for (const auto& item : list) {
    promotions.push_back(
        extensions::api::huhi_rewards::OnPromotions::PromotionsType());

    auto& promotion = promotions[promotions.size() -1];

    promotion.promotion_id = item->id;
    promotion.type = static_cast<int>(item->type);
    promotion.status = static_cast<int>(item->status);
    promotion.expires_at = item->expires_at;
    promotion.amount = item->approximate_value;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnPromotions::Create(
          static_cast<int>(result), promotions)
            .release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnPromotions::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPromotionFinished(
    RewardsService* rewards_service,
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router || result != ledger::type::Result::LEDGER_OK) {
    return;
  }

  extensions::api::huhi_rewards::OnPromotionFinish::
        Promotion promotion_api;

  promotion_api.promotion_id = promotion->id;
  promotion_api.type = static_cast<int>(promotion->type);
  promotion_api.status = static_cast<int>(promotion->status);
  promotion_api.expires_at = promotion->expires_at;
  promotion_api.amount = promotion->approximate_value;

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnPromotionFinish::Create
      (static_cast<int>(result), promotion_api).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnPromotionFinish::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnAdsEnabled(
    RewardsService* rewards_service,
    bool ads_enabled) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnAdsEnabled::Create(
          ads_enabled).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnAdsEnabled::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRewardsMainEnabled(
    RewardsService* rewards_service,
    bool rewards_main_enabled) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnEnabledMain::Create(
          rewards_main_enabled).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnEnabledMain::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPendingContributionSaved(
    RewardsService* rewards_service,
    const ledger::type::Result result) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnPendingContributionSaved::Create(
          static_cast<int>(result)).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnPendingContributionSaved::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPublisherListNormalized(
    RewardsService* rewards_service,
    ledger::type::PublisherInfoList list) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::vector<extensions::api::huhi_rewards::OnPublisherListNormalized::
        PublishersType> publishers;

  for (const auto& item : list) {
    publishers.push_back(
        extensions::api::huhi_rewards::OnPublisherListNormalized::
        PublishersType());

    auto& publisher = publishers[publishers.size() -1];

    publisher.publisher_key = item->id;
    publisher.percentage = item->percent;
    publisher.status = static_cast<int>(item->status);
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::
      OnPublisherListNormalized::Create(publishers)
          .release());

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnPublisherListNormalized::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnExcludedSitesChanged(
    RewardsService* rewards_service,
    std::string publisher_key,
    bool excluded) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::huhi_rewards::OnExcludedSitesChanged::Properties result;
  result.publisher_key = publisher_key;
  result.excluded = excluded;

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnExcludedSitesChanged::Create(result)
          .release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnExcludedSitesChanged::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRecurringTipSaved(
    RewardsService* rewards_service,
    bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnRecurringTipSaved::Create(
          success).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnRecurringTipSaved::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRecurringTipRemoved(
    RewardsService* rewards_service,
    bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnRecurringTipRemoved::Create(
          success).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnRecurringTipRemoved::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPendingContributionRemoved(
    RewardsService* rewards_service,
    const ledger::type::Result result) {
  extensions::EventRouter* event_router =
      extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnPendingContributionRemoved::Create(
          static_cast<int>(result)).release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnPendingContributionRemoved::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnReconcileComplete(
    RewardsService* rewards_service,
    const ledger::type::Result result,
    const std::string& contribution_id,
    const double amount,
    const ledger::type::RewardsType type,
    const ledger::type::ContributionProcessor processor) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::huhi_rewards::OnReconcileComplete::Properties properties;
  properties.result = static_cast<int>(result);
  properties.type = static_cast<int>(type);

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnReconcileComplete::Create(properties)
          .release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnReconcileComplete::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnDisconnectWallet(
      huhi_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& wallet_type) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::huhi_rewards::OnDisconnectWallet::Properties properties;
  properties.result = static_cast<int>(result);
  properties.wallet_type = wallet_type;

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnDisconnectWallet::Create(properties)
          .release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnDisconnectWallet::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnUnblindedTokensReady(
      huhi_rewards::RewardsService* rewards_service) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  auto args = std::make_unique<base::ListValue>();
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnUnblindedTokensReady::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnCompleteReset(const bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::huhi_rewards::OnCompleteReset::Properties properties;
  properties.success = success;

  std::unique_ptr<base::ListValue> args(
      extensions::api::huhi_rewards::OnCompleteReset::Create(properties)
          .release());
  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HUHI_START,
      extensions::api::huhi_rewards::OnCompleteReset::kEventName,
      std::move(args)));
  event_router->BroadcastEvent(std::move(event));
}

}  // namespace huhi_rewards