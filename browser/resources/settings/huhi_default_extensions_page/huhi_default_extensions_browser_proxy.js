/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// clang-format off
// #import {addSingletonGetter, sendWithPromise} from 'chrome://resources/js/cr.m.js';
// clang-format on

cr.define('settings', function () {
  /** @interface */
  /* #export */ class HuhiDefaultExtensionsBrowserProxy {
    /**
     * @param {boolean} value name.
     */
    setWebTorrentEnabled (value) {}
    setHuhiWalletEnabled (value) {}
    setHangoutsEnabled (value) {}
    setIPFSCompanionEnabled (value) {}
    setTorEnabled (value) {}
    isTorEnabled () {}
    isTorManaged () {}
    getRestartNeeded () {}
    getWeb3ProviderList () {}
    wasSignInEnabledAtStartup () {}
    getIPFSResolveMethodList () {}
    getIPFSEnabled () {}
  }

  /**
   * @implements {settings.HuhiDefaultExtensionsBrowserProxy}
   */
  /* #export */ class HuhiDefaultExtensionsBrowserProxyImpl {
    /** @override */
    setWebTorrentEnabled (value) {
      chrome.send('setWebTorrentEnabled', [value])
    }

    setHuhiWalletEnabled (value) {
      chrome.send('setHuhiWalletEnabled', [value])
    }

    setHangoutsEnabled (value) {
      chrome.send('setHangoutsEnabled', [value])
    }

    setIPFSCompanionEnabled (value) {
      chrome.send('setIPFSCompanionEnabled', [value])
    }

    setMediaRouterEnabled (value) {
      chrome.send('setMediaRouterEnabled', [value])
    }

    setTorEnabled (value) {
      chrome.send('setTorEnabled', [value])
    }

    isTorEnabled () {
      return cr.sendWithPromise('isTorEnabled')
    }

    isTorManaged () {
      return cr.sendWithPromise('isTorManaged')
    }

    getRestartNeeded () {
      return cr.sendWithPromise('getRestartNeeded')
    }

    /** @override */
    getWeb3ProviderList () {
      return new Promise(resolve => chrome.huhiWallet.getWeb3ProviderList(resolve))
    }

    wasSignInEnabledAtStartup () {
      return loadTimeData.getBoolean('signInAllowedOnNextStartupInitialValue')
    }

    /** @override */
    getIPFSResolveMethodList () {
      return new Promise(resolve => {
        if (!chrome.ipfs) {
          resolve(false)
          return
        }
        chrome.ipfs.getResolveMethodList(resolve)
      })
    }

    /** @override */
    getIPFSEnabled () {
      return new Promise(resolve => {
        if (!chrome.ipfs) {
          resolve(false)
          return
        }
        chrome.ipfs.getIPFSEnabled(resolve)
      })
    }
  }

  cr.addSingletonGetter(HuhiDefaultExtensionsBrowserProxyImpl)

  // #cr_define_end
  return {
    HuhiDefaultExtensionsBrowserProxy,
    HuhiDefaultExtensionsBrowserProxyImpl
  }
})
