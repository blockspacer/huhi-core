/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/common/resource_bundle_helper.h"

#include <string>

#include "base/command_line.h"
#include "base/path_service.h"
#include "chrome/common/chrome_paths.h"
#include "services/service_manager/embedder/switches.h"
#include "ui/base/resource/resource_bundle.h"

#if !defined(OS_IOS)
#include "content/public/common/content_switches.h"
#endif

#if defined(OS_MAC)
#include "base/mac/foundation_util.h"
#include "base/strings/sys_string_conversions.h"
#endif

#if defined(OS_ANDROID) && defined(HUHI_CHROMIUM_BUILD)
#include "ui/base/resource/resource_bundle_android.h"
#endif

namespace {

#if !defined(HUHI_CHROMIUM_BUILD) || !defined(OS_ANDROID)
base::FilePath GetResourcesPakFilePath() {
#if defined(OS_MAC)
  return base::mac::PathForFrameworkBundleResource(
      CFSTR("huhi_resources.pak"));
#else
  base::FilePath pak_path;
  base::PathService::Get(base::DIR_MODULE, &pak_path);
  pak_path = pak_path.AppendASCII("huhi_resources.pak");
  return pak_path;
#endif  // OS_MAC
}
#endif  // OS_ANDROID

#if !defined(OS_ANDROID)
base::FilePath GetScaledResourcesPakFilePath(ui::ScaleFactor scale_factor) {
  DCHECK(scale_factor == ui::SCALE_FACTOR_100P ||
         scale_factor == ui::SCALE_FACTOR_200P);

  const char* pak_file =
      (scale_factor == ui::SCALE_FACTOR_100P) ? "huhi_100_percent.pak"
                                              : "huhi_200_percent.pak";
#if defined(OS_MAC)
  base::ScopedCFTypeRef<CFStringRef> pak_file_mac(
      base::SysUTF8ToCFStringRef(pak_file));
  return base::mac::PathForFrameworkBundleResource(pak_file_mac);
#else
  base::FilePath pak_path;
  base::PathService::Get(base::DIR_MODULE, &pak_path);
  pak_path = pak_path.AppendASCII(pak_file);
  return pak_path;
#endif  // OS_MAC
}
#endif  // !defined(OS_ANDROID)

}  // namespace

namespace huhi {

void InitializeResourceBundle() {
#if defined(OS_ANDROID) && defined(HUHI_CHROMIUM_BUILD)
  ui::HuhiLoadMainAndroidPackFile("assets/huhi_resources.pak",
                                   base::FilePath());
  ui::HuhiLoadHuhi100PercentPackFile("assets/huhi_100_percent.pak",
                                       base::FilePath());
#else
  auto& rb = ui::ResourceBundle::GetSharedInstance();
  rb.AddDataPackFromPath(GetResourcesPakFilePath(), ui::SCALE_FACTOR_NONE);
  rb.AddDataPackFromPath(GetScaledResourcesPakFilePath(ui::SCALE_FACTOR_100P),
                         ui::SCALE_FACTOR_100P);
  if (ui::ResourceBundle::IsScaleFactorSupported(ui::SCALE_FACTOR_200P)) {
    rb.AddDataPackFromPath(GetScaledResourcesPakFilePath(ui::SCALE_FACTOR_200P),
                           ui::SCALE_FACTOR_200P);
  }
#endif  // OS_ANDROID && defined(HUHI_CHROMIUM_BUILD)
}

// Returns true if this subprocess type needs the ResourceBundle initialized
// and resources loaded.
bool SubprocessNeedsResourceBundle() {
  auto cmd = *base::CommandLine::ForCurrentProcess();
#if defined(OS_IOS)
  return false;
#else
  std::string process_type = cmd.GetSwitchValueASCII(switches::kProcessType);
  return
#if defined(OS_POSIX) && !defined(OS_MAC)
      // The zygote process opens the resources for the renderers.
      process_type == service_manager::switches::kZygoteProcess ||
#endif  // defined(OS_POSIX) && !defined(OS_MAC)
#if defined(OS_MAC)
      // Mac needs them too for scrollbar related images and for sandbox
      // profiles.
      process_type == switches::kPpapiPluginProcess ||
      process_type == switches::kPpapiBrokerProcess ||
      process_type == switches::kGpuProcess ||
#endif  // defined(OS_MAC)
      process_type == switches::kRendererProcess ||
      process_type == switches::kUtilityProcess;
#endif  // defined(OS_IOS)
}

}  // namespace huhi
