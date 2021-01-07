/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_COMPONENTS_HUHI_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_OBSERVER_H_
#define HUHI_COMPONENTS_HUHI_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_OBSERVER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/scoped_observer.h"
#include "huhi/components/huhi_component_updater/browser/local_data_files_service.h"

namespace huhi_component_updater {

// The abstract base class for observers of the local data files service,
// which is the component that arbitrates access to various DAT files
// like tracking protection.
class LocalDataFilesObserver {
 public:
  explicit LocalDataFilesObserver(
      LocalDataFilesService* local_data_files_service);
  virtual ~LocalDataFilesObserver();
  virtual void OnComponentReady(const std::string& component_id,
                                const base::FilePath& install_dir,
                                const std::string& manifest) = 0;
  virtual void OnLocalDataFilesServiceDestroyed();
  LocalDataFilesService* local_data_files_service();

 protected:
  LocalDataFilesService* local_data_files_service_;  // NOT OWNED
  ScopedObserver<LocalDataFilesService, LocalDataFilesObserver>
      local_data_files_observer_;
};

}  // namespace huhi_component_updater

#endif  // HUHI_COMPONENTS_HUHI_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_OBSERVER_H_
