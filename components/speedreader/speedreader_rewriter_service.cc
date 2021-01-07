/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/components/speedreader/speedreader_rewriter_service.h"

#include <utility>

#include "base/bind.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/task/post_task.h"
#include "huhi/components/speedreader/rust/ffi/speedreader.h"
#include "huhi/components/speedreader/speedreader_component.h"
#include "components/grit/huhi_components_resources.h"
#include "ui/base/resource/resource_bundle.h"
#include "url/gurl.h"

namespace speedreader {

namespace {

std::string GetDistilledPageStylesheet(const base::FilePath& stylesheet_path) {
  std::string stylesheet;
  const bool success = base::ReadFileToString(stylesheet_path, &stylesheet);

  if (!success || stylesheet.empty()) {
    VLOG(1) << "Failed to read stylesheet from component: " << stylesheet_path;
    stylesheet = ui::ResourceBundle::GetSharedInstance().LoadDataResourceString(
        IDR_SPEEDREADER_STYLE_DESKTOP);
  }

  return "<style id=\"huhi_speedreader_style\">" + stylesheet + "</style>";
}

}  // namespace

SpeedreaderRewriterService::SpeedreaderRewriterService(
    huhi_component_updater::HuhiComponent::Delegate* delegate)
    : component_(new speedreader::SpeedreaderComponent(delegate)),
      speedreader_(new speedreader::SpeedReader) {
  // Load the built-in stylesheet as the default
  content_stylesheet_ =
      "<style id=\"huhi_speedreader_style\">" +
      ui::ResourceBundle::GetSharedInstance().LoadDataResourceString(
          IDR_SPEEDREADER_STYLE_DESKTOP) +
      "</style>";

  // Check the paths from the component as observer may register
  // later than the paths were available in the component.
  const auto stylesheet_path = component_->GetStylesheetPath();
  if (!stylesheet_path.empty())
    OnStylesheetReady(stylesheet_path);

  const auto whitelist_path = component_->GetWhitelistPath();
  if (!whitelist_path.empty())
    OnWhitelistReady(whitelist_path);

  component_->AddObserver(this);
}

SpeedreaderRewriterService::~SpeedreaderRewriterService() {
  component_->RemoveObserver(this);
}

void SpeedreaderRewriterService::OnWhitelistReady(const base::FilePath& path) {
  VLOG(2) << "Whitelist ready at " << path;
  base::PostTaskAndReplyWithResult(
      FROM_HERE, {base::ThreadPool(), base::MayBlock()},
      base::BindOnce(
          &huhi_component_updater::LoadDATFileData<speedreader::SpeedReader>,
          path),
      base::BindOnce(&SpeedreaderRewriterService::OnLoadDATFileData,
                     weak_factory_.GetWeakPtr()));
}

void SpeedreaderRewriterService::OnStylesheetReady(const base::FilePath& path) {
  base::PostTaskAndReplyWithResult(
      FROM_HERE, {base::ThreadPool(), base::MayBlock()},
      base::BindOnce(&GetDistilledPageStylesheet, path),
      base::BindOnce(&SpeedreaderRewriterService::OnLoadStylesheet,
                     weak_factory_.GetWeakPtr()));
}

bool SpeedreaderRewriterService::IsWhitelisted(const GURL& url) {
  return speedreader_->IsReadableURL(url.spec());
}

std::unique_ptr<Rewriter> SpeedreaderRewriterService::MakeRewriter(
    const GURL& url) {
  return speedreader_->MakeRewriter(url.spec());
}

const std::string& SpeedreaderRewriterService::GetContentStylesheet() {
  return content_stylesheet_;
}

void SpeedreaderRewriterService::OnLoadStylesheet(std::string stylesheet) {
  VLOG(2) << "Speedreader stylesheet loaded";
  content_stylesheet_ = stylesheet;
}

void SpeedreaderRewriterService::OnLoadDATFileData(
    GetDATFileDataResult result) {
  VLOG(2) << "Speedreader loaded from DAT file";
  if (result.first)
    speedreader_ = std::move(result.first);
}

}  // namespace speedreader
