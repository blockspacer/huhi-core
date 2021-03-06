/**
 * Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions.editurl;

import android.content.Context;

import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.omnibox.suggestions.OmniboxSuggestion;
import org.chromium.chrome.browser.omnibox.suggestions.UrlBarDelegate;
import org.chromium.chrome.browser.omnibox.suggestions.basic.SuggestionHost;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.favicon.LargeIconBridge;

public class HuhiEditUrlSuggestionProcessor extends EditUrlSuggestionProcessor {
    private boolean mHasClearedOmniboxForFocus;

    public HuhiEditUrlSuggestionProcessor(Context context, SuggestionHost suggestionHost,
            UrlBarDelegate locationBarDelegate, Supplier<LargeIconBridge> iconBridgeSupplier,
            Supplier<Tab> tabSupplier, Supplier<ShareDelegate> shareDelegateSupplier) {
        super(context, suggestionHost, locationBarDelegate, iconBridgeSupplier, tabSupplier,
                shareDelegateSupplier);
    }

    @Override
    public boolean doesProcessSuggestion(OmniboxSuggestion suggestion, int position) {
        // We don't want to clear omnibox for focus, so just pretend that it's already happened
        mHasClearedOmniboxForFocus = true;
        return super.doesProcessSuggestion(suggestion, position);
    }
}
