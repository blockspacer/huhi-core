/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/timer.h"

#include <utility>

#include "huhi_base/random.h"

namespace ads {

Timer::Timer()
    : timer_(std::make_unique<base::OneShotTimer>()) {
  DCHECK(timer_);
}

Timer::~Timer() {
  Stop();
}

void Timer::set_timer_for_testing(
    std::unique_ptr<base::OneShotTimer> timer) {
  timer_ = std::move(timer);
}

base::Time Timer::Start(
    const base::TimeDelta& delay,
    base::OnceClosure user_task) {
  Stop();

  timer_->Start(FROM_HERE, delay, std::move(user_task));

  const base::Time time = base::Time::Now() + delay;

  return time;
}

base::Time Timer::StartWithPrivacy(
    const base::TimeDelta& delay,
    base::OnceClosure user_task) {
  const int64_t delay_as_int64 = static_cast<int64_t>(delay.InSeconds());
  const uint64_t rand_delay = huhi_base::random::Geometric(delay_as_int64);

  return Start(base::TimeDelta::FromSeconds(rand_delay), std::move(user_task));
}

bool Timer::IsRunning() const {
  return timer_->IsRunning();
}

void Timer::FireNow() {
  return timer_->FireNow();
}

void Timer::Stop() {
  if (!IsRunning()) {
    return;
  }

  timer_->Stop();
}

}  // namespace ads
