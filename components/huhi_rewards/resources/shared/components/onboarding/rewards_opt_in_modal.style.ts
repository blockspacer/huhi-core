/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import modalBackground from './assets/opt_in_modal_bg.svg'

export const root = styled.div`
  flex: 0 0 auto;
  max-width: 335px;
  padding: 17px;
  font-family: var(--huhi-font-heading);
  text-align: center;
  background-color: var(--huhi-palette-white);
  background-image: url(${modalBackground});
  background-repeat: no-repeat;
  background-position: 4px -11px;
  background-size: auto 200px;
  box-shadow: 0px 0px 16px rgba(99, 105, 110, 0.2);
  border-radius: 8px;
`

export const close = styled.div`
  color: var(--huhi-palette-neutral600);
  text-align: right;

  button {
    margin: 0;
    padding: 2px;
    background: none;
    border: none;
    cursor: pointer;
  }

  .icon {
    display: block;
    width: 14px;
    height: auto;
  }
`

export const header = styled.div`
  margin-top: 17px;
  color: var(--huhi-palette-black);
  font-weight: 600;
  font-size: 18px;
  line-height: 22px;
`

export const batIcon = styled.div`
  display: inline-block;
  vertical-align: middle;
  width: 26px;
  margin-right: 7px;
`

export const text = styled.div`
  margin: 8px 6px 0;
  color: var(--huhi-palette-neutral700);
  font-size: 14px;
  line-height: 24px;
`

export const enable = styled.div`
  margin-top: 40px;
`

// NOTE: The "Add Funds" flow is currently disabled
export const addFunds = styled.div`
  display: none;

  margin-top: 18px;
  color: var(--huhi-color-brandBat);

  button {
    font-weight: 600;
    font-size: 14px;
    line-height: 21px;
    border: 0;
    background: 0;
    margin: 0;
    padding: 0;
    cursor: pointer;
  }
`

export const addFundsIcon = styled.span`
  display: inline-block;
  width: 16px;
  height: 16px;
  vertical-align: middle;
  margin-right: 5px;
  margin-bottom: 2px;
`

export const terms = styled.div`
  margin: 32px 14px 15px;
  color: var(--huhi-palette-neutral600);
  font-size: 11px;
  line-height: 16px;

  a {
    color: var(--huhi-color-brandBat);
    font-weight: 600;
    text-decoration: none;
  }
`

export const addFundsItem = styled.div`
  margin-top: 24px;
  color: var(--huhi-palette-neutral900);
  font-weight: 600;
  font-size: 14px;
  line-height: 20px;

  .icon {
    color: var(--huhi-palette-neutral600);
    width: 24px;
    vertical-align: middle;
    margin-right: 16px;
    margin-bottom: 3px;
  }
`

export const addFundsAction = styled.div`
  margin-top: 31px;
`
