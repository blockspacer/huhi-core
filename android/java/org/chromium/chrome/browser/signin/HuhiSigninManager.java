/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.signin;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.browser.externalauth.ExternalAuthUtils;
import org.chromium.chrome.browser.sync.AndroidSyncSettings;
import org.chromium.components.signin.AccountTrackerService;
import org.chromium.components.signin.identitymanager.IdentityManager;
import org.chromium.components.signin.identitymanager.IdentityMutator;

public class HuhiSigninManager extends SigninManager {
    HuhiSigninManager(long nativeSigninManagerAndroid, AccountTrackerService accountTrackerService,
            IdentityManager identityManager, IdentityMutator identityMutator,
            AndroidSyncSettings androidSyncSettings, ExternalAuthUtils externalAuthUtils) {
        super(nativeSigninManagerAndroid, accountTrackerService, identityManager, identityMutator,
                androidSyncSettings, externalAuthUtils);
    }

    @Override
    public boolean isSignInAllowed() {
        return false;
    }

    @Override
    public boolean isSigninSupported() {
        return false;
    }

    @CalledByNative
    private static SigninManager create(long nativeSigninManagerAndroid,
            AccountTrackerService accountTrackerService, IdentityManager identityManager,
            IdentityMutator identityMutator) {
        assert nativeSigninManagerAndroid != 0;
        assert accountTrackerService != null;
        assert identityManager != null;
        assert identityMutator != null;
        return new HuhiSigninManager(nativeSigninManagerAndroid, accountTrackerService,
                identityManager, identityMutator,
                AndroidSyncSettings.get(), ExternalAuthUtils.getInstance());
    }
}
