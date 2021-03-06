/* This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

chrome.test.runTests([
  function notHuhiShieldsExtensionHasAccess() {
    if (chrome.huhiShields) {
      chrome.test.fail();
    } else {
      chrome.test.succeed();
    }
  },
  function notHuhiShieldsHasNoWalletAccess() {
    if (!chrome.huhiWallet) {
      chrome.test.succeed();
    } else {
      chrome.test.fail();
    }
  },
  function extensionsHaveNoBinanceAccess() {
    if (!chrome.binance) {
      chrome.test.succeed();
    } else {
      chrome.test.fail();
    }
  },
]);
