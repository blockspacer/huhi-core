/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { action } from 'typesafe-actions'

// Constants
import { types } from '../constants/ipfs_types'

export const getConnectedPeers = () => action(types.IPFS_GET_CONNECTED_PEERS)

export const onGetConnectedPeers = (peerCount: number) =>
  action(types.IPFS_ON_GET_CONNECTED_PEERS, {
    peerCount
  })

export const getAddressesConfig = () => action(types.IPFS_GET_ADDRESSES_CONFIG)

export const onGetAddressesConfig = (addressesConfig: IPFS.AddressesConfig) =>
  action(types.IPFS_ON_GET_ADDRESSES_CONFIG, {
    addressesConfig
  })

export const getDaemonStatus = () => action(types.IPFS_GET_DAEMON_STATUS)

export const onGetDaemonStatus = (daemonStatus: IPFS.DaemonStatus) =>
  action(types.IPFS_ON_GET_DAEMON_STATUS, {
    daemonStatus
  })

export const launchDaemon = () => action(types.IPFS_LAUNCH_DAEMON)
export const shutdownDaemon = () => action(types.IPFS_SHUTDOWN_DAEMON)
