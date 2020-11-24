/* This Source Code Form is subject to the terms of the Huhi Software
+ * License, v. 2.0. If a copy of the MPL was not distributed with this file,
+ * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"  // For OS_LINUX

#if defined(OS_LINUX)
#include "huhi/browser/ui/views/huhi_views_delegate_linux.h"
#undef ChromeViewsDelegate
#define ChromeViewsDelegate HuhiViewsDelegateLinux
#endif

#include "../../../../../../chrome/browser/ui/views/chrome_browser_main_extra_parts_views.cc"
