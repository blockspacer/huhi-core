/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "huhi/browser/huhi_stats/huhi_stats_updater.h"

#include <utility>

#include "base/command_line.h"
#include "base/system/sys_info.h"
#include "huhi/browser/huhi_stats/huhi_stats_updater_params.h"
#include "huhi/browser/huhi_stats/switches.h"
#include "huhi/browser/version_info.h"
#include "huhi/common/huhi_channel_info.h"
#include "huhi/common/network_constants.h"
#include "huhi/common/pref_names.h"
#include "huhi/components/huhi_referrals/buildflags/buildflags.h"
#include "huhi/components/huhi_stats/browser/huhi_stats_updater_util.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/common/channel_info.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/load_flags.h"
#include "net/base/url_util.h"
#include "net/http/http_response_headers.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/mojom/fetch_api.mojom-shared.h"

#if BUILDFLAG(ENABLE_HUHI_REFERRALS)
#include "huhi/components/huhi_referrals/common/pref_names.h"
#endif

namespace huhi_stats {

namespace {
// Ping the update server shortly after startup.
static constexpr int kUpdateServerStartupPingDelaySeconds = 3;

// Every five minutes, check if we need to ping the update server for
// today.
static constexpr int kUpdateServerPeriodicPingFrequencySeconds = 5 * 60;

static constexpr int kMinimumUsageThreshold = 3;

GURL GetUpdateURL(
    const GURL& base_update_url,
    const huhi_stats::HuhiStatsUpdaterParams& stats_updater_params) {
  GURL update_url(base_update_url);
  update_url = net::AppendQueryParameter(update_url, "platform",
                                         huhi_stats::GetPlatformIdentifier());
  update_url =
      net::AppendQueryParameter(update_url, "channel", huhi::GetChannelName());
  update_url = net::AppendQueryParameter(
      update_url, "version",
      version_info::GetHuhiVersionWithoutChromiumMajorVersion());
  update_url = net::AppendQueryParameter(update_url, "daily",
                                         stats_updater_params.GetDailyParam());
  update_url = net::AppendQueryParameter(update_url, "weekly",
                                         stats_updater_params.GetWeeklyParam());
  update_url = net::AppendQueryParameter(
      update_url, "monthly", stats_updater_params.GetMonthlyParam());
  update_url = net::AppendQueryParameter(
      update_url, "first", stats_updater_params.GetFirstCheckMadeParam());
  update_url = net::AppendQueryParameter(
      update_url, "woi", stats_updater_params.GetWeekOfInstallationParam());
  update_url = net::AppendQueryParameter(
      update_url, "dtoi", stats_updater_params.GetDateOfInstallationParam());
  update_url = net::AppendQueryParameter(
      update_url, "ref", stats_updater_params.GetReferralCodeParam());
  return update_url;
}

net::NetworkTrafficAnnotationTag AnonymousStatsAnnotation() {
  return net::DefineNetworkTrafficAnnotation("huhi_stats_updater", R"(
    semantics {
      sender:
        "Huhi Stats Updater"
      description:
        "This service sends anonymous usage statistics to Huhi."
      trigger:
        "Stats are automatically sent at intervals while Huhi "
        "is running."
      data: "Anonymous usage statistics."
      destination: WEBSITE
    }
    policy {
      cookies_allowed: NO
      setting:
        "This feature cannot be disabled by settings."
      policy_exception_justification:
        "Not implemented."
    })");
}
}  // anonymous namespace

HuhiStatsUpdater::HuhiStatsUpdater(PrefService* pref_service)
    : pref_service_(pref_service) {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kHuhiStatsUpdaterServer)) {
    usage_server_ =
        command_line.GetSwitchValueASCII(switches::kHuhiStatsUpdaterServer);
    if (!GURL(usage_server_).is_valid())
      LOG(ERROR) << "bad url given as stats updater url: " << usage_server_;
  } else {
    usage_server_ = HUHI_USAGE_SERVER;
  }
}

HuhiStatsUpdater::~HuhiStatsUpdater() {}

void HuhiStatsUpdater::Start() {
  // Startup timer, only initiated once we've checked for a promo
  // code.
  DCHECK(!server_ping_startup_timer_);
  server_ping_startup_timer_ = std::make_unique<base::OneShotTimer>();

  QueueServerPing();

  // Periodic timer.
  DCHECK(!server_ping_periodic_timer_);
  server_ping_periodic_timer_ = std::make_unique<base::RepeatingTimer>();
  server_ping_periodic_timer_->Start(
      FROM_HERE,
      base::TimeDelta::FromSeconds(kUpdateServerPeriodicPingFrequencySeconds),
      this, &HuhiStatsUpdater::OnServerPingTimerFired);
}

void HuhiStatsUpdater::Stop() {
  server_ping_startup_timer_.reset();
  server_ping_periodic_timer_.reset();
}

bool HuhiStatsUpdater::MaybeDoThresholdPing(int score) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  threshold_score_ += score;

  // We only do this once
  if (HasDoneThresholdPing())
    return true;

  // We don't want to start the threshold ping if:
  //   (1) The standard ping is still waiting to be sent.
  //   (2) Referrals haven't initialized, thus blocking the standard ping.
  // The standard usage ping will set the url and call us back.
  if (server_ping_startup_timer_->IsRunning() || !IsReferralInitialized())
    return threshold_score_ >= kMinimumUsageThreshold;

  if (threshold_score_ >= kMinimumUsageThreshold) {
    SendUserTriggeredPing();
    return true;
  }
  return false;
}

void HuhiStatsUpdater::SetStatsUpdatedCallback(
    StatsUpdatedCallback stats_updated_callback) {
  stats_updated_callback_ = std::move(stats_updated_callback);
}

void HuhiStatsUpdater::SetStatsThresholdCallback(
    StatsUpdatedCallback stats_threshold_callback) {
  stats_threshold_callback_ = std::move(stats_threshold_callback);
}

GURL HuhiStatsUpdater::BuildStatsEndpoint(const std::string& path) {
  auto stats_updater_url = GURL(usage_server_ + path);
#if defined(OFFICIAL_BUILD)
  CHECK(stats_updater_url.is_valid());
#endif
  return stats_updater_url;
}

void HuhiStatsUpdater::OnSimpleLoaderComplete(
    std::unique_ptr<huhi_stats::HuhiStatsUpdaterParams> stats_updater_params,
    scoped_refptr<net::HttpResponseHeaders> headers) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  GURL final_url = simple_url_loader_->GetFinalURL();
  int response_code = -1;
  if (headers)
    response_code = headers->response_code();
  if (simple_url_loader_->NetError() != net::OK || response_code < 200 ||
      response_code > 299) {
    VLOG(1) << "Failed to send usage stats to update server"
            << ", error: " << simple_url_loader_->NetError()
            << ", response code: " << response_code
            << ", url: " << final_url.spec();
    return;
  }

  bool first_check_made = pref_service_->GetBoolean(kFirstCheckMade);

  // We need to set this *before* params are saved.
  if (!first_check_made && !HasDoneThresholdPing()) {
    auto endpoint = BuildStatsEndpoint(kHuhiUsageThresholdPath);
    auto threshold_query = GetUpdateURL(endpoint, *stats_updater_params);
    // Unfortunately we need to serialize this in case the user starts
    // the browser, stats ping goes, then we lose the original params.
    pref_service_->SetString(kThresholdQuery, threshold_query.spec());
  }

  // The request to the update server succeeded, so it's safe to save
  // the usage preferences now.
  stats_updater_params->SavePrefs();

  // Inform the client that the stats ping completed, if requested.
  if (!stats_updated_callback_.is_null())
    stats_updated_callback_.Run(final_url);

  // In case the first call was blocked by our timer.
  (void)MaybeDoThresholdPing(0);

  // Log the full URL of the stats ping.
  VLOG(1) << "Huhi stats ping, url: " << final_url.spec();
}

void HuhiStatsUpdater::OnThresholdLoaderComplete(
    scoped_refptr<net::HttpResponseHeaders> headers) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  GURL final_url = simple_url_loader_->GetFinalURL();
  int response_code = -1;
  if (headers)
    response_code = headers->response_code();
  if (simple_url_loader_->NetError() != net::OK || response_code < 200 ||
      response_code > 299) {
    VLOG(1) << "Failed to send threshold ping to update server"
            << ", error: " << simple_url_loader_->NetError()
            << ", response code: " << response_code
            << ", url: " << final_url.spec();
    return;
  }

  // Inform the client that the threshold ping completed, if requested.
  if (!stats_threshold_callback_.is_null())
    stats_threshold_callback_.Run(final_url);

  // We only send this query once.
  DisableThresholdPing();

  // Log the full URL of the stats ping.
  VLOG(1) << "Huhi stats ping, url: " << final_url.spec();
}

void HuhiStatsUpdater::OnServerPingTimerFired() {
  // If we already pinged the stats server today, then we're done.
  std::string today_ymd = huhi_stats::GetDateAsYMD(base::Time::Now());
  std::string last_check_ymd = pref_service_->GetString(kLastCheckYMD);
  if (base::CompareCaseInsensitiveASCII(today_ymd, last_check_ymd) == 0)
    return;

  SendServerPing();
}

bool HuhiStatsUpdater::IsReferralInitialized() {
#if BUILDFLAG(ENABLE_HUHI_REFERRALS)
  return pref_service_->GetBoolean(kReferralInitialization) ||
         pref_service_->GetBoolean(kReferralCheckedForPromoCodeFile);
#else
  return true;
#endif
}

bool HuhiStatsUpdater::HasDoneThresholdPing() {
  return pref_service_->GetBoolean(kThresholdCheckMade);
}

void HuhiStatsUpdater::DisableThresholdPing() {
  pref_service_->SetBoolean(kThresholdCheckMade, true);
  pref_service_->ClearPref(kThresholdQuery);
}

void HuhiStatsUpdater::QueueServerPing() {
  if (IsReferralInitialized()) {
    StartServerPingStartupTimer();
  } else {
    pref_change_registrar_.reset(new PrefChangeRegistrar());
    pref_change_registrar_->Init(pref_service_);
    pref_change_registrar_->Add(
        kReferralInitialization,
        base::Bind(&HuhiStatsUpdater::OnReferralInitialization,
                   base::Unretained(this)));
  }
}  // namespace huhi

void HuhiStatsUpdater::OnReferralInitialization() {
  StartServerPingStartupTimer();
}

void HuhiStatsUpdater::StartServerPingStartupTimer() {
  server_ping_startup_timer_->Start(
      FROM_HERE,
      base::TimeDelta::FromSeconds(kUpdateServerStartupPingDelaySeconds), this,
      &HuhiStatsUpdater::OnServerPingTimerFired);
}

void HuhiStatsUpdater::SendServerPing() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto traffic_annotation = AnonymousStatsAnnotation();
  auto resource_request = std::make_unique<network::ResourceRequest>();
  auto stats_updater_params =
      std::make_unique<huhi_stats::HuhiStatsUpdaterParams>(pref_service_);

  auto endpoint = BuildStatsEndpoint(kHuhiUsageStandardPath);
  resource_request->url = GetUpdateURL(endpoint, *stats_updater_params);
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->load_flags = net::LOAD_DO_NOT_SAVE_COOKIES |
                                 net::LOAD_BYPASS_CACHE |
                                 net::LOAD_DISABLE_CACHE;
  resource_request->headers.SetHeader("X-Huhi-API-Key",
                                      huhi_stats::GetAPIKey());
  network::mojom::URLLoaderFactory* loader_factory =
      g_browser_process->system_network_context_manager()
          ->GetURLLoaderFactory();
  simple_url_loader_ = network::SimpleURLLoader::Create(
      std::move(resource_request), traffic_annotation);
  simple_url_loader_->DownloadHeadersOnly(
      loader_factory,
      base::BindOnce(&HuhiStatsUpdater::OnSimpleLoaderComplete,
                     base::Unretained(this), std::move(stats_updater_params)));
}

void HuhiStatsUpdater::SendUserTriggeredPing() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto traffic_annotation = AnonymousStatsAnnotation();
  auto resource_request = std::make_unique<network::ResourceRequest>();

  // This pref is only set when kFirstCheckMade is false,
  // so if it is empty, we have an existing user. Disable
  // threshold ping and don't send a request.
  auto threshold_query = pref_service_->GetString(kThresholdQuery);
  if (threshold_query.empty())
    DisableThresholdPing();

  resource_request->url = GURL(threshold_query);
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->load_flags = net::LOAD_DO_NOT_SAVE_COOKIES |
                                 net::LOAD_BYPASS_CACHE |
                                 net::LOAD_DISABLE_CACHE;
  resource_request->headers.SetHeader("X-Huhi-API-Key",
                                      huhi_stats::GetAPIKey());
  network::mojom::URLLoaderFactory* loader_factory =
      g_browser_process->system_network_context_manager()
          ->GetURLLoaderFactory();
  simple_url_loader_ = network::SimpleURLLoader::Create(
      std::move(resource_request), traffic_annotation);
  simple_url_loader_->DownloadHeadersOnly(
      loader_factory,
      base::BindOnce(&HuhiStatsUpdater::OnThresholdLoaderComplete,
                     base::Unretained(this)));
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HuhiStatsUpdater> HuhiStatsUpdaterFactory(
    PrefService* pref_service) {
  return std::make_unique<HuhiStatsUpdater>(pref_service);
}

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kFirstCheckMade, false);
  registry->RegisterBooleanPref(kThresholdCheckMade, false);
  registry->RegisterStringPref(kThresholdQuery, std::string());
  registry->RegisterIntegerPref(kLastCheckWOY, 0);
  registry->RegisterIntegerPref(kLastCheckMonth, 0);
  registry->RegisterStringPref(kLastCheckYMD, std::string());
  registry->RegisterStringPref(kWeekOfInstallation, std::string());
}
}  // namespace huhi_stats
