/* This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

// Components
import Hero from '../hero'
import Button from 'huhi-ui/components/buttonsIndicators/button'
import InfoCard, { CardProps } from '../infoCard'
import {
  AdsMegaphoneIcon,
  ArrowDownIcon,
  BatColorIcon,
  LoaderIcon,
  RewardsActivateIcon,
  RewardsSendTipsIcon
} from 'huhi-ui/components/icons'
import { Alert, RewardsButton, SettingsPage } from '../'

// Utils
import { getLocale } from 'huhi-ui/helpers'

// Styles
import {
  StyledOptInSection,
  StyledOptInInnerSection,
  StyledCenterSection,
  StyledCenterContent,
  StyledSection,
  StyledCenterInner,
  StyledInfoContent,
  StyledTakeActionContent,
  StyledBackground,
  StyledBatLogo,
  StyledRewardsTitle,
  StyledActionTitle,
  StyledCenterTitle,
  StyledSubTitle,
  StyledTrademark,
  StyledRewardsParagraph,
  StyledTeaserParagraph,
  StyledCenterParagraph,
  StyledAnchor,
  StyledOptInSecond,
  StyledHeroInfo,
  StyledAlert,
  StyledAlertLeft,
  StyledAlertContent,
  StyledTOSWrapper,
  StyledServiceText,
  StyledServiceLink
} from './style'

export interface Props {
  id?: string
  optInAction: () => void
  creating?: boolean
  onlyAnonWallet?: boolean
  onReTry?: () => void
  onTOSClick?: () => void
  onPrivacyClick?: () => void
}

class WelcomePage extends React.PureComponent<Props, {}> {
  private isTouchScreen: boolean
  private centerTextSection: HTMLDivElement | null

  constructor (props: Props) {
    super(props)
    this.centerTextSection = null
    this.isTouchScreen = 'ontouchstart' in window
  }

  scrollToCenter = () => {
    if (!this.centerTextSection) {
      return
    }

    const centerTextSection = this.centerTextSection
    if (centerTextSection) {
      window.scrollTo({
        behavior: 'smooth',
        top: centerTextSection.offsetTop
      })
    }
  }

  refSet = (node: HTMLDivElement) => {
    this.centerTextSection = node
  }

  optInAction = () => {
    this.props.optInAction()
  }

  hero = () => {
    const { onTOSClick, onPrivacyClick } = this.props

    return (
      <Hero
        id={'rewards-hero'}
        isMobile={this.isTouchScreen}
      >
        <StyledSection>
          <StyledBatLogo>
            <BatColorIcon />
          </StyledBatLogo>
          <StyledHeroInfo>
            <StyledRewardsTitle level={2}>
              {getLocale('huhiRewardsTitle')}
            </StyledRewardsTitle>
            <StyledTrademark>TM</StyledTrademark>
            <StyledSubTitle level={4}>
              {getLocale('huhiRewardsSubTitle')}
            </StyledSubTitle>
            <StyledRewardsParagraph>
              {getLocale('huhiRewardsDesc')}
            </StyledRewardsParagraph>
          </StyledHeroInfo>
        </StyledSection>
        <StyledOptInSection>
          {
            this.props.creating
              ? <RewardsButton
                type={'opt-in'}
                disabled={true}
                testId={'optInAction'}
                text={getLocale('huhiRewardsCreatingText')}
                icon={<LoaderIcon />}
              />
              : <RewardsButton
                type={'opt-in'}
                onClick={this.optInAction}
                testId={'optInAction'}
                text={getLocale('huhiRewardsOptInText')}
              />
          }
        </StyledOptInSection>
        {
          !this.isTouchScreen
          ? <StyledTOSWrapper header={true}>
              <StyledServiceText header={true}>
                {getLocale('serviceTextWelcome')} <StyledServiceLink header={true} onClick={onTOSClick}>{getLocale('termsOfService')}</StyledServiceLink> {getLocale('and')} <StyledServiceLink header={true} onClick={onPrivacyClick}>{getLocale('privacyPolicy')}</StyledServiceLink>.
              </StyledServiceText>
            </StyledTOSWrapper>
          : null
        }
        <StyledSection>
          <StyledTeaserParagraph>
            {getLocale('huhiRewardsTeaser')}
          </StyledTeaserParagraph>
          <StyledAnchor onClick={this.scrollToCenter}>
            <ArrowDownIcon />
          </StyledAnchor>
        </StyledSection>
      </Hero>
    )
  }

  get centerTextContent () {
    return (
      <StyledCenterContent>
        <StyledCenterInner>
          <StyledCenterTitle level={3}>
            {getLocale('whyHuhiRewards')}
          </StyledCenterTitle>
          <StyledCenterParagraph>
            {getLocale('whyHuhiRewardsDesc1')}
          </StyledCenterParagraph>
          <StyledCenterParagraph>
            {getLocale('whyHuhiRewardsDesc2')}
          </StyledCenterParagraph>
        </StyledCenterInner>
      </StyledCenterContent>
    )
  }

  optInContent = () => {
    const { onPrivacyClick, onTOSClick } = this.props

    return (
      <StyledOptInInnerSection>
        <StyledActionTitle level={4}>
          {getLocale('readyToTakePart')}
        </StyledActionTitle>
        <StyledOptInSecond>
          {
            this.props.creating
              ? <RewardsButton
                type={'cta-opt-in'}
                disabled={true}
                text={getLocale('huhiRewardsCreatingText')}
                icon={<LoaderIcon />}
              />
              : <RewardsButton
                type={'cta-opt-in'}
                onClick={this.optInAction}
                text={getLocale('readyToTakePartOptInText')}
              />
          }
        </StyledOptInSecond>
        {
          !this.isTouchScreen
          ? <StyledTOSWrapper>
              <StyledServiceText>
                {getLocale('serviceTextReady')} <StyledServiceLink onClick={onTOSClick}>{getLocale('termsOfService')}</StyledServiceLink> {getLocale('and')} <StyledServiceLink onClick={onPrivacyClick}>{getLocale('privacyPolicy')}</StyledServiceLink>.
              </StyledServiceText>
            </StyledTOSWrapper>
          : null
        }
      </StyledOptInInnerSection>
    )
  }

  infoCards = (): CardProps[] => {
    const { onlyAnonWallet } = this.props
    return [
      {
        title: getLocale('turnOnRewardsTitle'),
        description: getLocale('turnOnRewardsDesc'),
        icon: <RewardsActivateIcon />
      },
      {
        title: getLocale('huhiAdsTitle'),
        description: onlyAnonWallet ? getLocale('huhiAdsDescPoints') : getLocale('huhiAdsDesc'),
        icon: <AdsMegaphoneIcon />
      },
      {
        title: getLocale('huhiContributeTitle'),
        description: getLocale('huhiContributeDesc'),
        icon: <RewardsSendTipsIcon />
      }
    ]
  }

  get welcomePageContent () {
    return (
      <>
        {
          this.props.onReTry
            ? <StyledAlert>
              <Alert type={'error'}>
                <StyledAlertContent>
                  <StyledAlertLeft>
                    <b>{getLocale('walletFailedTitle')}</b><br />{getLocale('walletFailedText')}
                  </StyledAlertLeft>
                  <Button
                    level={'primary'}
                    type={'accent'}
                    text={getLocale('walletFailedButton')}
                    onClick={this.props.onReTry}
                  />
                </StyledAlertContent>
              </Alert>
            </StyledAlert>
            : null
        }
        <StyledBackground>
          <StyledSection>
            {this.hero()}
          </StyledSection>
          <StyledCenterSection>
            <StyledCenterSection innerRef={this.refSet}>
              {this.centerTextContent}
            </StyledCenterSection>
            <StyledInfoContent>
              <InfoCard
                id='rewards-info'
                cards={this.infoCards()}
              />
            </StyledInfoContent>
            <StyledTakeActionContent>
              {this.optInContent()}
            </StyledTakeActionContent>
          </StyledCenterSection>
        </StyledBackground>
      </>
    )
  }

  render () {
    const { id } = this.props

    // We don't need the SettingsPage wrapper on touchscreen devices
    if (this.isTouchScreen) {
      return this.welcomePageContent
    }

    return (
      <SettingsPage id={id}>
        {this.welcomePageContent}
      </SettingsPage>
    )
  }
}

export default WelcomePage
