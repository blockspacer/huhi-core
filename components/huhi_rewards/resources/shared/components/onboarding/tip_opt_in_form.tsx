/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { BatColorIcon } from 'huhi-ui/components/icons'

import { LocaleContext } from '../../lib/locale_context'
import { TermsOfService } from './terms_of_service'
import { MainButton } from './main_button'

import * as style from './tip_opt_in_form.style'

interface Props {
  onEnable: () => void
  onDismiss: () => void
}

export function TipOptInForm (props: Props) {
  const { getString } = React.useContext(LocaleContext)
  return (
    <style.root>
      <style.content>
        <style.batIcon><BatColorIcon /></style.batIcon>
        <style.heading>{getString('onboardingTipHeader')}</style.heading>
        <style.text>
          {getString('onboardingTipText')}
        </style.text>
        <style.enable>
          <MainButton onClick={props.onEnable}>
            {getString('onboardingStartUsingRewards')}
          </MainButton>
        </style.enable>
        <style.dismiss>
          <button onClick={props.onDismiss}>
            {getString('onboardingMaybeLater')}
          </button>
        </style.dismiss>
      </style.content>
      <style.footer>
        <TermsOfService />
      </style.footer>
    </style.root>
  )
}
