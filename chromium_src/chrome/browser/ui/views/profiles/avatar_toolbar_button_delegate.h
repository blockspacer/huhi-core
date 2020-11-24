/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
#define HUHI_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_

// Pull in all original includes: Init and GetState are too common - we don't
// want to redefine them elsewhere.
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observer.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/ui/avatar_button_error_controller.h"
#include "chrome/browser/ui/avatar_button_error_controller_delegate.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/views/profiles/avatar_toolbar_button.h"
#include "components/signin/public/identity_manager/identity_manager.h"
#include "ui/gfx/image/image.h"

#define GetState virtual GetState
#define Init virtual Init
#include "../../../../../../../chrome/browser/ui/views/profiles/avatar_toolbar_button_delegate.h"
#undef Init
#undef GetState

class HuhiAvatarToolbarButtonDelegate : public AvatarToolbarButtonDelegate {
 public:
  using AvatarToolbarButtonDelegate::AvatarToolbarButtonDelegate;
  ~HuhiAvatarToolbarButtonDelegate() override = default;

  void Init(AvatarToolbarButton* button, Profile* profile) override;
  gfx::Image GetGaiaAccountImage() const;
  AvatarToolbarButton::State GetState() const override;

 private:
  Profile* profile_ = nullptr;
  DISALLOW_COPY_AND_ASSIGN(HuhiAvatarToolbarButtonDelegate);
};

#undef HUHI_PROFILE_MENU_VIEW_H

#endif  // HUHI_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
