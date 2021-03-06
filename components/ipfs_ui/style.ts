/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'huhi-ui/theme'
import Button, { Props as ButtonProps } from 'huhi-ui/components/buttonsIndicators/button'

export const Section = styled<{}, 'div'>('div')`
  margin: 0 20px 10px;
`

export const Title = styled<{}, 'h4'>('h4')`
  margin: 15px 0 5px;
`

export const SideBySideButtons = styled<{}, 'div'>('div')`
  box-sizing: border-box;
  display: flex;
  flex-direction: row;
  margin-top: 10px;
`

export const PaddedButton = styled(Button as React.ComponentType<ButtonProps>)`
  margin: 5px;
`
