/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_BROWSER_IPFS_CONTENT_BROWSER_CLIENT_HELPER_H_
#define HUHI_BROWSER_IPFS_CONTENT_BROWSER_CLIENT_HELPER_H_

#include "base/optional.h"
#include "content/public/browser/web_contents.h"

class GURL;

namespace content {
class BrowserContext;
}  // namespace content

namespace ui {
enum PageTransition;
}  // namespace ui

namespace url {
class Origin;
}  // namespace url

namespace ipfs {

class ContentBrowserClientHelper {
 public:
  static bool TranslateIPFSURL(const GURL& url, GURL* new_url, bool local);

  static bool HandleIPFSURLReverseRewrite(GURL* url,
      content::BrowserContext* browser_context);

  static void LoadOrLaunchIPFSURL(
      const GURL& url,
      content::WebContents::OnceGetter web_contents_getter,
      ui::PageTransition page_transition,
      bool has_user_gesture,
      const base::Optional<url::Origin>& initiating_origin);

  static bool HandleIPFSURLRewrite(GURL* url,
      content::BrowserContext* browser_context);

  static void HandleIPFSProtocol(
      const GURL& url,
      content::WebContents::OnceGetter web_contents_getter,
      ui::PageTransition page_transition,
      bool has_user_gesture,
      const base::Optional<url::Origin>& initiating_origin);

  static bool IsIPFSProtocol(const GURL& url);
};

}  // namespace ipfs

#endif  // HUHI_BROWSER_IPFS_CONTENT_BROWSER_CLIENT_HELPER_H_
