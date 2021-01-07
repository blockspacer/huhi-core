// Copyright (c) 2020 The Huhi Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import styled from 'huhi-ui/theme'
import {
  Block as StandardBlock,
  Heading as StandardHeading,
  Image as StandardImage,
  Text as StandardText,
  Time as StandardTime
} from './default'

export const Large = styled(StandardBlock.withComponent('article'))`
  padding: 0;
`

export const Medium = styled(StandardBlock.withComponent('article'))`
  padding: 0;
`

export const Small = styled(StandardBlock.withComponent('article'))`
  padding: 0;
  width: 100%;
  min-height: 310px;
`

export const DealsCard = styled(StandardBlock)`
`

export const Content = styled<{}, 'div'>('div')`
  box-sizing: border-box;
  padding: 25px 35px;
  ${Small} & {
    padding: 20px 28px;
  }
`

type ImageFrameProps = {
  isImageLoaded: boolean
}

export const ImageFrame = styled<ImageFrameProps, 'figure'>('figure')`
  opacity: ${p => p.isImageLoaded ? 1 : 0};
  margin: 0;
  width: 100%;
  position: relative;
  padding-top: 67%;
  display: block;
  height: auto;
  overflow: hidden;
  transition: opacity 240ms ease-out;
`

export const ListImageFrame = styled(ImageFrame)`
  height: 100%;
  padding-top: 0;
`

export const Image = styled(StandardImage)`
  display: block;
  position: absolute;
  border: none;
  background: none;
  top: 0;
  bottom: 0;
  left: 0;
  right: 0;
  width: 100%;
  height: 100%;
  object-fit: cover;
  background-color: rgba(188,188,188,0.2);
`

export const Heading = styled(StandardHeading)`
  font-weight: 600;
  a {
    display: block;
    color: inherit;
    text-decoration: none;
  }
`

export const PublisherHeading = styled(StandardHeading)`
  font-weight: 600;
`

export const Text = styled(StandardText)`
  font-family: ${p => p.theme.fontFamily.heading};
  font-size: 18px;
  line-height: 25px;
  font-weight: 600;

  a {
    display: block;
    color: inherit;
    text-decoration: none;
  }
`

export const Time = styled(StandardTime)`
  ${Large} & {
    margin-top: 8px;
  }
`

export const Publisher = styled('div')`
  box-sizing: border-box;
  margin: 10px 0 0 0;
  padding: 0;
  font: 500 14px ${p => p.theme.fontFamily.heading};
  color: #fff;
`

export const ContainerForTwo = styled<{}, 'div'>('div')`
  width: 680px;
  display: grid;
  grid-template-columns: 1fr 1fr;
  grid-gap: 30px;
`

export const DealsCategory = styled('h3')`
  margin: 0;
  font: 600 18px ${p => p.theme.fontFamily.heading};
  color: white;
`

export const DealsList = styled('div')`
  margin-top: 24px;
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  grid-gap: 30px;
`

export const DealItem = styled('a')`
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  align-items: stretch;
  gap: 8px;
`

export const DealDescription = styled(Time)`
  margin: 0;
`
