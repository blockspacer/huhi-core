/**
 * Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.onboarding;

import android.app.Activity;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

import org.chromium.chrome.R;
import org.chromium.base.ApplicationStatus;
import org.chromium.chrome.browser.app.HuhiActivity;
import org.chromium.chrome.browser.onboarding.NonSwipeableViewPager;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.OnboardingViewPagerAdapter;

public class OnboardingActivity extends AppCompatActivity implements OnViewPagerAction {
    private NonSwipeableViewPager viewPager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_onboarding);

        OnboardingPrefManager.getInstance().setOnboardingShown(true);

        OnboardingViewPagerAdapter onboardingViewPagerAdapter = new OnboardingViewPagerAdapter(
            this, getSupportFragmentManager(), this);
        viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(onboardingViewPagerAdapter);
        if (HuhiActivity.getHuhiActivity() != null ) {
            HuhiActivity.getHuhiActivity().hideRewardsOnboardingIcon();
        }
    }

    @Override
    public void onSkip() {
        finish();
    }

    @Override
    public void onNext() {
        viewPager.setCurrentItem(viewPager.getCurrentItem() + 1);
    }

    @Override
    public void onContinueToWallet() {
        if (HuhiActivity.getHuhiActivity() != null ) {
            HuhiActivity.getHuhiActivity().openRewardsPanel();
        }
        finish();
    }

    @Override
    public void onBackPressed() {

    }

    static public OnboardingActivity getOnboardingActivity() {
        for (Activity ref : ApplicationStatus.getRunningActivities()) {
            if (!(ref instanceof OnboardingActivity)) continue;

            return (OnboardingActivity)ref;
        }

        return null;
    }
}
