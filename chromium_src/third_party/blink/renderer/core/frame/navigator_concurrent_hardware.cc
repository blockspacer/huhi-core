/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <random>

#include "base/system/sys_info.h"
#include "huhi/third_party/blink/renderer/huhi_farbling_constants.h"
#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/navigator_concurrent_hardware.h"

namespace huhi {

using blink::LocalFrame;

const unsigned kFakeMinProcessors = 2;
const unsigned kFakeMaxProcessors = 8;

unsigned FarbleNumberOfProcessors(LocalFrame* frame) {
  unsigned true_value =
      static_cast<unsigned>(base::SysInfo::NumberOfProcessors());
  if ((true_value <= 2) || !frame || !frame->GetContentSettingsClient())
    return true_value;
  unsigned farbled_value = true_value;
  switch (frame->GetContentSettingsClient()->GetHuhiFarblingLevel()) {
    case HuhiFarblingLevel::OFF: {
      break;
    }
    case HuhiFarblingLevel::MAXIMUM: {
      true_value = kFakeMaxProcessors;
      // "Maximum" behavior is "balanced" behavior but with a fake maximum,
      // so fall through here.
      U_FALLTHROUGH;
    }
    case HuhiFarblingLevel::BALANCED: {
      std::mt19937_64 prng = HuhiSessionCache::From(*(frame->GetDocument()))
                                 .MakePseudoRandomGenerator();
      farbled_value =
          kFakeMinProcessors + (prng() % (true_value + 1 - kFakeMinProcessors));
      break;
    }
    default:
      NOTREACHED();
  }
  return farbled_value;
}

}  // namespace huhi

namespace blink {

unsigned NavigatorConcurrentHardware::hardwareConcurrency(
    ScriptState* script_state) const {
  LocalFrame* frame = nullptr;
  if (LocalDOMWindow* window = LocalDOMWindow::From(script_state))
    frame = window->GetFrame();
  return huhi::FarbleNumberOfProcessors(frame);
}

}  // namespace blink
