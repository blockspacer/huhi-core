/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_UI_VIEWS_TABS_HUHI_NEW_TAB_BUTTON_H_
#define HUHI_BROWSER_UI_VIEWS_TABS_HUHI_NEW_TAB_BUTTON_H_

#include "chrome/browser/ui/views/tabs/new_tab_button.h"
#include "third_party/skia/include/core/SkPath.h"

class TabStrip;
namespace views {
class ButtonListener;
}

class HuhiNewTabButton : public NewTabButton {
 public:
  static const gfx::Size kButtonSize;
  using NewTabButton::NewTabButton;

 protected:
  void PaintIcon(gfx::Canvas* canvas) override;

 private:
  gfx::Size CalculatePreferredSize() const override;
  SkPath GetBorderPath(const gfx::Point& origin,
                       float scale,
                       bool extend_to_top) const override;
  DISALLOW_COPY_AND_ASSIGN(HuhiNewTabButton);
};

#endif  // HUHI_BROWSER_UI_VIEWS_TABS_HUHI_NEW_TAB_BUTTON_H_
