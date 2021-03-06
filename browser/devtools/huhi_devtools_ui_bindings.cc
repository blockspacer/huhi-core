/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/devtools/huhi_devtools_ui_bindings.h"

#include <string>

#include "base/values.h"
#include "huhi/browser/themes/huhi_dark_mode_utils.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace {
std::string GetDevToolsUIThemeValue() {
  dark_mode::HuhiDarkModeType theme_type =
      dark_mode::GetActiveHuhiDarkModeType();
  // In devtools' theme, default is translated to light.
  return theme_type == dark_mode::HuhiDarkModeType::HUHI_DARK_MODE_TYPE_DARK ?
      "\"dark\"" : "\"default\"";
}
}

void HuhiDevToolsUIBindings::GetPreferences(const DispatchCallback& callback) {
  const base::DictionaryValue* prefs =
      profile_->GetPrefs()->GetDictionary(prefs::kDevToolsPreferences);

  if (prefs->FindKey("uiTheme"))
    return DevToolsUIBindings::GetPreferences(callback);

  base::Value new_prefs(prefs->Clone());
  new_prefs.SetKey("uiTheme", base::Value(GetDevToolsUIThemeValue()));
  callback.Run(&new_prefs);
}
