/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp_background_images.model;

public class BackgroundImage extends NTPImage{
    private int imageDrawable;
    private int centerPoint;
    private ImageCredit imageCredit;

    public BackgroundImage(int imageDrawable, int centerPoint, ImageCredit imageCredit) {
        this.imageDrawable = imageDrawable;
        this.centerPoint = centerPoint;
        this.imageCredit = imageCredit;
    }

    public BackgroundImage(int imageDrawable, int centerPoint) {
        this.imageDrawable = imageDrawable;
        this.centerPoint = centerPoint;
    }

    public int getImageDrawable() {
        return imageDrawable;
    }

    public int getCenterPoint() {
        return centerPoint;
    }

    public ImageCredit getImageCredit() {
        return imageCredit;
    }
}