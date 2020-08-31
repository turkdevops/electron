// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "shell/browser/serial/electron_serial_delegate.h"

#include <utility>

#include "shell/browser/serial/serial_chooser_context.h"
#include "shell/browser/serial/serial_chooser_controller.h"

namespace electron {

ElectronSerialDelegate::ElectronSerialDelegate() = default;

ElectronSerialDelegate::~ElectronSerialDelegate() = default;

std::unique_ptr<content::SerialChooser> ElectronSerialDelegate::RunChooser(
    content::RenderFrameHost* frame,
    std::vector<blink::mojom::SerialPortFilterPtr> filters,
    content::SerialChooser::Callback callback) {
  chooser_controller_ = std::make_unique<SerialChooserController>(
      frame, std::move(filters), std::move(callback));
  return nullptr;
}

bool ElectronSerialDelegate::CanRequestPortPermission(
    content::RenderFrameHost* frame) {
  return true;
}

bool ElectronSerialDelegate::HasPortPermission(
    content::RenderFrameHost* frame,
    const device::mojom::SerialPortInfo& port) {
  return true;
}

device::mojom::SerialPortManager* ElectronSerialDelegate::GetPortManager(
    content::RenderFrameHost* frame) {
  return SerialChooserContext::GetInstance()->GetPortManager();
}

void ElectronSerialDelegate::AddObserver(content::RenderFrameHost* frame,
                                         Observer* observer) {
  return SerialChooserContext::GetInstance()->AddPortObserver(observer);
}

void ElectronSerialDelegate::RemoveObserver(content::RenderFrameHost* frame,
                                            Observer* observer) {
  return SerialChooserContext::GetInstance()->RemovePortObserver(observer);
}

}  // namespace electron
