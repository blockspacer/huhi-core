/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/extensions/huhi_wallet_util.h"

#include <memory>

#include "base/environment.h"

namespace extensions {

bool HasInfuraProjectID() {
  std::string project_id = GetInfuraProjectID();

  if (!project_id.empty()) {
    return true;
  }

  std::unique_ptr<base::Environment> env(base::Environment::Create());
  bool has_project_id = env->HasVar("HUHI_INFURA_PROJECT_ID");

  return has_project_id;
}

std::string GetInfuraProjectID() {
  std::string project_id(HUHI_INFURA_PROJECT_ID);
  std::unique_ptr<base::Environment> env(base::Environment::Create());

  if (env->HasVar("HUHI_INFURA_PROJECT_ID")) {
    env->GetVar("HUHI_INFURA_PROJECT_ID", &project_id);
  }

  return project_id;
}

std::string GetHuhiKey() {
  std::string huhi_key(HUHI_SERVICES_KEY);
  std::unique_ptr<base::Environment> env(base::Environment::Create());

  if (env->HasVar("HUHI_SERVICES_KEY")) {
    env->GetVar("HUHI_SERVICES_KEY", &huhi_key);
  }

  return huhi_key;
}

}  // namespace extensions
