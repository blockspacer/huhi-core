/* Copyright (c) 2020 The Huhi Software Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Huhi Software
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define REGISTER_HUHI_SCHEMES_DISPLAY_ISOLATED_AND_NO_JS \
  WebString huhi_scheme(WebString::FromASCII(kHuhiUIScheme)); \
  WebSecurityPolicy::RegisterURLSchemeAsDisplayIsolated(huhi_scheme); \
  WebSecurityPolicy::RegisterURLSchemeAsNotAllowingJavascriptURLs( \
      huhi_scheme); \
                     \
  WebString binance_scheme(WebString::FromASCII(kBinanceScheme)); \
  WebSecurityPolicy::RegisterURLSchemeAsDisplayIsolated(binance_scheme); \
  WebSecurityPolicy::RegisterURLSchemeAsNotAllowingJavascriptURLs( \
      binance_scheme); \
                       \
  WebString gemini_scheme(WebString::FromASCII(kGeminiScheme)); \
  WebSecurityPolicy::RegisterURLSchemeAsDisplayIsolated(gemini_scheme); \
  WebSecurityPolicy::RegisterURLSchemeAsNotAllowingJavascriptURLs( \
      gemini_scheme);

#include "../../../../content/renderer/render_thread_impl.cc"
