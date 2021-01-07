// Copyright (c) 2020 The Huhi Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { OnReadFeedItem } from './'

export default function useReadArticleClickHandler (action: OnReadFeedItem, item: HuhiToday.FeedItem) {
  return React.useCallback((e: React.MouseEvent) => {
    const openInNewTab = e.ctrlKey || e.metaKey
    action({ item, openInNewTab })
    e.preventDefault()
  }, [action, item])
}
