// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

namespace blink {
class LocalFrame;

namespace {
class MediaKeySystemAccessInitializer;

void MaybeOnWidevineRequest(MediaKeySystemAccessInitializer* initializer,
                            LocalFrame* frame);
}  // namespace
}  // namespace blink

// Notify Huhi about the widevine availability request.
#define HUHI_NAVIGATOR_REQUEST_MEDIA_KEY_SYSTEM_ACCESS \
  MaybeOnWidevineRequest(initializer, window->GetFrame());

#include "../../../../../../../third_party/blink/renderer/modules/encryptedmedia/navigator_request_media_key_system_access.cc"  // NOLINT
#undef HUHI_NAVIGATOR_REQUEST_MEDIA_KEY_SYSTEM_ACCESS

#include "huhi/components/huhi_drm/huhi_drm.mojom-blink.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"

namespace blink {
namespace {

// Notifies Huhi about the widevine availability request.
void MaybeOnWidevineRequest(MediaKeySystemAccessInitializer* initializer,
                            LocalFrame* frame) {
  if (initializer->KeySystem() == "com.widevine.alpha") {
    if (frame->Client()->GetRemoteNavigationAssociatedInterfaces()) {
      mojo::AssociatedRemote<huhi_drm::mojom::blink::HuhiDRM>
          huhi_drm_binding;
      frame->Client()->GetRemoteNavigationAssociatedInterfaces()->GetInterface(
          &huhi_drm_binding);
      DCHECK(huhi_drm_binding.is_bound());
      huhi_drm_binding->OnWidevineKeySystemAccessRequest();
    }
  }
}

}  // namespace
}  // namespace blink