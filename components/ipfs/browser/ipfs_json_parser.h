/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_COMPONENTS_IPFS_BROWSER_IPFS_JSON_PARSER_H_
#define HUHI_COMPONENTS_IPFS_BROWSER_IPFS_JSON_PARSER_H_

#include <string>
#include <vector>

#include "huhi/components/ipfs/browser/addresses_config.h"

class IPFSJSONParser {
 public:
  static bool GetPeersFromJSON(const std::string& json,
                               std::vector<std::string>* peers);
  static bool GetAddressesConfigFromJSON(const std::string& json,
                                         ipfs::AddressesConfig* config);
};

#endif  // HUHI_COMPONENTS_IPFS_BROWSER_IPFS_JSON_PARSER_H_
