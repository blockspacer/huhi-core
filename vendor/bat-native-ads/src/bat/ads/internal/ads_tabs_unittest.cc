/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads_impl.h"

#include <memory>

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/files/scoped_temp_dir.h"
#include "base/test/task_environment.h"
#include "huhi/components/l10n/browser/locale_helper_mock.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/unittest_util.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

// npm run test -- huhi_unit_tests --filter=BatAds*

namespace ads {

class BatAdsTabsTest : public ::testing::Test {
 protected:
  BatAdsTabsTest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME),
        ads_client_mock_(std::make_unique<NiceMock<AdsClientMock>>()),
        ads_(std::make_unique<AdsImpl>(ads_client_mock_.get())),
        locale_helper_mock_(std::make_unique<
            NiceMock<huhi_l10n::LocaleHelperMock>>()),
        platform_helper_mock_(std::make_unique<
            NiceMock<PlatformHelperMock>>()) {
    // You can do set-up work for each test here

    huhi_l10n::LocaleHelper::GetInstance()->set_for_testing(
        locale_helper_mock_.get());

    PlatformHelper::GetInstance()->set_for_testing(platform_helper_mock_.get());
  }

  ~BatAdsTabsTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)

    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    const base::FilePath path = temp_dir_.GetPath();

    ON_CALL(*ads_client_mock_, IsEnabled())
        .WillByDefault(Return(true));

    ON_CALL(*ads_client_mock_, ShouldAllowAdConversionTracking())
        .WillByDefault(Return(true));

    SetBuildChannel(false, "test");

    ON_CALL(*locale_helper_mock_, GetLocale())
        .WillByDefault(Return("en-US"));

    MockPlatformHelper(platform_helper_mock_, PlatformType::kMacOS);

    ads_->OnWalletUpdated("c387c2d8-a26d-4451-83e4-5c0c6fd942be",
        "5BEKM1Y7xcRSg/1q8in/+Lki2weFZQB+UMYZlRw8ql8=");

    MockLoad(ads_client_mock_);
    MockLoadUserModelForId(ads_client_mock_);
    MockLoadResourceForId(ads_client_mock_);
    MockSave(ads_client_mock_);

    database_ = std::make_unique<Database>(path.AppendASCII("database.sqlite"));
    MockRunDBTransaction(ads_client_mock_, database_);

    Initialize(ads_);
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case

  base::test::TaskEnvironment task_environment_;

  base::ScopedTempDir temp_dir_;

  std::unique_ptr<AdsClientMock> ads_client_mock_;
  std::unique_ptr<AdsImpl> ads_;
  std::unique_ptr<huhi_l10n::LocaleHelperMock> locale_helper_mock_;
  std::unique_ptr<PlatformHelperMock> platform_helper_mock_;
  std::unique_ptr<Database> database_;
};

TEST_F(BatAdsTabsTest,
    MediaIsPlaying) {
  // Arrange
  ads_->OnTabUpdated(1, "https://huhisoft.com", true, true, false);
  ads_->OnMediaPlaying(1);

  // Act
  const bool is_media_playing = ads_->IsMediaPlaying();

  // Assert
  EXPECT_TRUE(is_media_playing);
}

TEST_F(BatAdsTabsTest,
    MediaIsNotPlaying) {
  // Arrange
  ads_->OnTabUpdated(1, "https://huhisoft.com", true, true, false);

  ads_->OnMediaPlaying(1);
  ads_->OnMediaPlaying(2);

  ads_->OnMediaStopped(1);
  ads_->OnMediaStopped(2);

  // Act
  const bool is_playing = ads_->IsMediaPlaying();

  // Assert
  EXPECT_FALSE(is_playing);
}

TEST_F(BatAdsTabsTest,
    IncognitoTabUpdated) {
  // Arrange
  EXPECT_CALL(*ads_client_mock_, Log(_, _, _, _))
      .Times(0);

  // Act
  ads_->OnTabUpdated(1, "https://huhisoft.com", true, true, true);

  // Assert
}

TEST_F(BatAdsTabsTest,
    InactiveIncognitoTabUpdated) {
  // Arrange
  EXPECT_CALL(*ads_client_mock_, Log(_, _, _, _))
      .Times(0);

  // Act
  ads_->OnTabUpdated(1, "https://huhisoft.com", false, false, true);

  // Assert
}

TEST_F(BatAdsTabsTest,
    TabUpdated) {
  // Arrange
  EXPECT_CALL(*ads_client_mock_, Log(_, _, _, _))
      .Times(1);

  // Act
  ads_->OnTabUpdated(1, "https://huhisoft.com", true, true, false);

  // Assert
}

TEST_F(BatAdsTabsTest,
    InactiveTabUpdated) {
  // Arrange
  EXPECT_CALL(*ads_client_mock_, Log(_, _, _, _))
      .Times(1);

  // Act
  ads_->OnTabUpdated(1, "https://huhisoft.com", false, false, false);

  // Assert
}

TEST_F(BatAdsTabsTest,
    TabClosedWhileMediaIsPlaying) {
  // Arrange
  ads_->OnMediaPlaying(1);

  // Act
  ads_->OnTabClosed(1);

  // Assert
  EXPECT_FALSE(ads_->IsMediaPlaying());
}

}  // namespace ads
