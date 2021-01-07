/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_INFOBARS_HUHI_CONFIRM_P3A_INFOBAR_DELEGATE_H_
#define HUHI_BROWSER_INFOBARS_HUHI_CONFIRM_P3A_INFOBAR_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/strings/string16.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "url/gurl.h"

class InfoBarService;
class PrefService;

// An infobar that is run with a string, buttons, and a "Learn More" link.
class HuhiConfirmP3AInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  static void Create(InfoBarService* infobar_service, PrefService* local_state);

 private:
  explicit HuhiConfirmP3AInfoBarDelegate(PrefService* local_state);
  ~HuhiConfirmP3AInfoBarDelegate() override;

  infobars::InfoBarDelegate::InfoBarIdentifier GetIdentifier() const override;
  const gfx::VectorIcon& GetVectorIcon() const override;
  bool ShouldExpire(const NavigationDetails& details) const override;
  void InfoBarDismissed() override;
  base::string16 GetMessageText() const override;
  int GetButtons() const override;
  base::string16 GetButtonLabel(InfoBarButton button) const override;
  base::string16 GetLinkText() const override;
  GURL GetLinkURL() const override;
  bool Accept() override;
  bool Cancel() override;

  PrefService* local_state_;

  DISALLOW_COPY_AND_ASSIGN(HuhiConfirmP3AInfoBarDelegate);
};

#endif  // HUHI_BROWSER_INFOBARS_HUHI_CONFIRM_P3A_INFOBAR_DELEGATE_H_
