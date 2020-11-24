/* This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

// Feature-specific components
import { Content, Title, Paragraph, PrimaryButton } from '../../../components'

// Utils
import locale from '../fakeLocale'

// Images
import { WelcomeRewardsImage } from '../../../components/images'

interface Props {
  index: number
  currentScreen: number
  onClick: () => void
}

export default class PaymentsBox extends React.PureComponent<Props, {}> {
  render () {
    const { index, currentScreen, onClick } = this.props
    return (
      <Content
        zIndex={index}
        active={index === currentScreen}
        screenPosition={'1' + (index + 1) + '0%'}
        isPrevious={index <= currentScreen}
      >
        <WelcomeRewardsImage />
        <Title>{locale.enableHuhiRewards}</Title>
        <Paragraph>{locale.setupHuhiRewards}</Paragraph>
        <PrimaryButton
          level='primary'
          type='accent'
          size='large'
          text={locale.getStarted}
          onClick={onClick}
        />
      </Content>
    )
  }
}