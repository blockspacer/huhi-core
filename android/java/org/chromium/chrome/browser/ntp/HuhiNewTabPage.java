/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp;

import android.app.Activity;

import androidx.annotation.Nullable;

import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.browser_controls.BrowserControlsStateProvider;
import org.chromium.chrome.browser.compositor.layouts.OverviewModeBehavior;
import org.chromium.chrome.browser.feed.HuhiFeedSurfaceCoordinator;
import org.chromium.chrome.browser.feed.FeedProcessScopeFactory;
import org.chromium.chrome.browser.feed.shared.FeedSurfaceProvider;
import org.chromium.chrome.browser.lifecycle.ActivityLifecycleDispatcher;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.ui.native_page.NativePageHost;
import org.chromium.components.browser_ui.bottomsheet.BottomSheetController;

public class HuhiNewTabPage extends NewTabPage {
    private NewTabPageLayout mNewTabPageLayout;
    private FeedSurfaceProvider mFeedSurfaceProvider;

    public HuhiNewTabPage(Activity activity,
            BrowserControlsStateProvider browserControlsStateProvider,
            Supplier<Tab> activityTabProvider, SnackbarManager snackbarManager,
            ActivityLifecycleDispatcher lifecycleDispatcher, TabModelSelector tabModelSelector,
            boolean isTablet, NewTabPageUma uma, boolean isInNightMode,
            NativePageHost nativePageHost, Tab tab, BottomSheetController bottomSheetController) {
        super(activity, browserControlsStateProvider, activityTabProvider, snackbarManager,
                lifecycleDispatcher, tabModelSelector, isTablet, uma, isInNightMode, nativePageHost,
                tab, bottomSheetController);

        assert mNewTabPageLayout instanceof HuhiNewTabPageLayout;
        if (mNewTabPageLayout instanceof HuhiNewTabPageLayout) {
            ((HuhiNewTabPageLayout) mNewTabPageLayout).setTab(tab);
        }
    }

    @Override
    protected void initializeMainView(Activity activity, Supplier<Tab> tabProvider,
            SnackbarManager snackbarManager, TabModelSelector tabModelSelector, NewTabPageUma uma,
            boolean isInNightMode, BottomSheetController bottomSheetController) {
        super.initializeMainView(activity, tabProvider, snackbarManager, tabModelSelector, uma,
                isInNightMode, bottomSheetController);
        // Override surface provider
        Profile profile = Profile.fromWebContents(mTab.getWebContents());

        assert !FeedProcessScopeFactory.isFeedProcessEnabled();
        mFeedSurfaceProvider = new HuhiFeedSurfaceCoordinator(activity, snackbarManager,
                tabModelSelector, tabProvider,
                new SnapScrollHelper(mNewTabPageManager, mNewTabPageLayout), mNewTabPageLayout,
                null, null, isInNightMode, this, mNewTabPageManager.getNavigationDelegate(),
                profile, false, bottomSheetController);
    }
}
