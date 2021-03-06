/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_IMPL_H_
#define HUHI_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_IMPL_H_

#include "components/permissions/permission_request.h"

#define GetIconId                 \
  GetIconId_ChromiumImpl() const; \
  IconId GetIconId

#define GetMessageTextFragment                 \
  GetMessageTextFragment_ChromiumImpl() const; \
  base::string16 GetMessageTextFragment

#include "../../../../components/permissions/permission_request_impl.h"
#undef GetMessageTextFragment
#undef GetIconId

#endif  // HUHI_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_IMPL_H_
