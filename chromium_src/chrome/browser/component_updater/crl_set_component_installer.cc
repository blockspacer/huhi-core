/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define RegisterCRLSetComponent RegisterCRLSetComponent_ChromiumImpl
#include "../../../../../chrome/browser/component_updater/crl_set_component_installer.cc"  // NOLINT
#undef RegisterCRLSetComponent

#include "chrome/browser/browser_process.h"

#if !defined(OS_ANDROID)
#include "chrome/browser/component_updater/component_updater_utils.h"
#include "extensions/common/constants.h"
#endif

namespace component_updater {

void OnCRLSetRegistered() {
// https://github.com/huhisoftware/browser-android-tabs/issues/857
#if !defined(OS_ANDROID)
  component_updater::HuhiOnDemandUpdate(crl_set_extension_id);
#endif
}

void RegisterCRLSetComponent(ComponentUpdateService* cus,
                             const base::FilePath& user_data_dir) {
  auto installer = base::MakeRefCounted<component_updater::ComponentInstaller>(
      std::make_unique<CRLSetPolicy>());
  installer->Register(g_browser_process->component_updater(),
                      base::Bind(&OnCRLSetRegistered));
}

}  // namespace component_updater
