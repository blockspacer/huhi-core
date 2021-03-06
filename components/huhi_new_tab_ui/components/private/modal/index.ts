/* This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { ComponentType } from 'react'
import styled from 'huhi-ui/theme'
import DefaultModal, { Props } from 'huhi-ui/components/popupModals/modal'
import Heading from 'huhi-ui/components/text/heading'

export const Modal = styled(DefaultModal as ComponentType<Props>)`
  height: 100%;
`

export const LimitedBounds = styled<{}, 'article'>('article')`
  overflow: auto;
  height: 500px;
  padding: 20px;
`

export const HeadingText = styled(Heading)`
  font-size: 20px;
  font-weight: 500;
`

export const Paragraph = styled<{}, 'p'>('p')`
  font-size: 15px;
  font-family: ${p => p.theme.fontFamily.body};
  letter-spacing: .3px;
  line-height: 24px;
`

export const Footer = styled<{}, 'footer'>('footer')`
  display: flex;
  justify-content: flex-end;
`
