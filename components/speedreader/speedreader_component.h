/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HUHI_COMPONENTS_SPEEDREADER_SPEEDREADER_COMPONENT_H_
#define HUHI_COMPONENTS_SPEEDREADER_SPEEDREADER_COMPONENT_H_

#include <memory>
#include <string>

#include "base/files/file_path_watcher.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "huhi/components/huhi_component_updater/browser/huhi_component.h"

namespace base {
class FilePath;
}

class GURL;

namespace speedreader {

class SpeedreaderComponent
    : public huhi_component_updater::HuhiComponent,
      public base::SupportsWeakPtr<SpeedreaderComponent> {
 public:
  class Observer : public base::CheckedObserver {
   public:
    virtual void OnWhitelistReady(const base::FilePath& path) = 0;
    virtual void OnStylesheetReady(const base::FilePath& path) = 0;

   protected:
    ~Observer() override = default;
  };

  explicit SpeedreaderComponent(Delegate* delegate);
  ~SpeedreaderComponent() override;

  SpeedreaderComponent(const SpeedreaderComponent&) = delete;
  SpeedreaderComponent& operator=(const SpeedreaderComponent&) = delete;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  const base::FilePath& GetWhitelistPath() { return whitelist_path_; }
  const base::FilePath& GetStylesheetPath() { return stylesheet_path_; }

 private:
  // huhi_component_updater::HuhiComponent:
  void OnComponentReady(const std::string& component_id,
                        const base::FilePath& install_dir,
                        const std::string& manifest) override;

  // Used in testing/development with custom rule set for auto-reloading
  void OnWhitelistFileReady(const base::FilePath& path, bool error);

  base::ObserverList<Observer> observers_;
  std::unique_ptr<base::FilePathWatcher> whitelist_path_watcher_;
  base::FilePath whitelist_path_;
  base::FilePath stylesheet_path_;
  base::WeakPtrFactory<SpeedreaderComponent> weak_factory_{this};
};

}  // namespace speedreader

#endif  // HUHI_COMPONENTS_SPEEDREADER_SPEEDREADER_COMPONENT_H_