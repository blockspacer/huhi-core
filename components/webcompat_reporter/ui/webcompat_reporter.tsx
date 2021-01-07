/* Copyright (c) 2020 The Huhi Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { bindActionCreators } from 'redux'

import Theme from 'huhi-ui/theme/huhi-default'
import DarkTheme from 'huhi-ui/theme/huhi-dark'
import HuhiCoreThemeProvider from '../../common/HuhiCoreThemeProvider'
require('../../../ui/webui/resources/fonts/muli.css')
require('../../../ui/webui/resources/fonts/poppins.css')

// Containers
import App from './containers/App'

// Utils
import store from './store'
import * as webcompatReporterActions from './actions/webcompatreporter_actions'

let actions: any

function getActions () {
  if (actions) {
    return actions
  }

  actions = bindActionCreators(webcompatReporterActions, store.dispatch.bind(store))
  return actions
}

function loadDialogArgs () {
  const dialogArgsRaw = chrome.getVariableValue('dialogArguments')
  let dialogArgs
  try {
    dialogArgs = JSON.parse(dialogArgsRaw)
  } catch (e) {
    console.error('Error parsing incoming dialog args: ', dialogArgsRaw, e)
  }

  getActions().setSiteUrl(dialogArgs.siteUrl)
}

function initialize () {
  loadDialogArgs()

  new Promise(resolve => chrome.huhiTheme.getHuhiThemeType(resolve))
  .then((themeType: chrome.huhiTheme.ThemeType) => {
    render(
      <Provider store={store}>
        <HuhiCoreThemeProvider
          initialThemeType={themeType}
          dark={DarkTheme}
          light={Theme}
        >
          <App />
        </HuhiCoreThemeProvider>
      </Provider>,
      document.getElementById('root')
    )
  })
  .catch(error => {
    console.error('Problem mounting webcompat reporter modal', error)
  })
}

document.addEventListener('DOMContentLoaded', initialize)
