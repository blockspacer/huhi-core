/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_AD_EVENTS_AD_NOTIFICATIONS_AD_NOTIFICATION_EVENT_DISMISSED_H_  // NOLINT
#define BAT_ADS_INTERNAL_AD_EVENTS_AD_NOTIFICATIONS_AD_NOTIFICATION_EVENT_DISMISSED_H_  // NOLINT

#include "bat/ads/internal/ad_events/ad_event.h"

namespace ads {

class AdsImpl;
struct AdNotificationInfo;

namespace ad_notifications {

class AdEventDismissed : public AdEvent<AdNotificationInfo> {
 public:
  AdEventDismissed(
      AdsImpl* ads);

  ~AdEventDismissed() override;

  void Trigger(
      const AdNotificationInfo& ad) override;

 private:
  AdsImpl* ads_;  // NOT OWNED
};

}  // namespace ad_notifications
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_AD_EVENTS_AD_NOTIFICATIONS_AD_NOTIFICATION_EVENT_DISMISSED_H_  // NOLINT
