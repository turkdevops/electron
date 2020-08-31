// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_SERIAL_ELECTRON_SERIAL_DELEGATE_H_
#define SHELL_BROWSER_SERIAL_ELECTRON_SERIAL_DELEGATE_H_

#include <memory>
#include <vector>

#include "content/public/browser/serial_delegate.h"
#include "shell/browser/serial/serial_chooser_controller.h"

namespace electron {

class ElectronSerialDelegate : public content::SerialDelegate {
 public:
  ElectronSerialDelegate();
  ~ElectronSerialDelegate() override;

  std::unique_ptr<content::SerialChooser> RunChooser(
      content::RenderFrameHost* frame,
      std::vector<blink::mojom::SerialPortFilterPtr> filters,
      content::SerialChooser::Callback callback) override;
  bool CanRequestPortPermission(content::RenderFrameHost* frame) override;
  bool HasPortPermission(content::RenderFrameHost* frame,
                         const device::mojom::SerialPortInfo& port) override;
  device::mojom::SerialPortManager* GetPortManager(
      content::RenderFrameHost* frame) override;
  void AddObserver(content::RenderFrameHost* frame,
                   Observer* observer) override;
  void RemoveObserver(content::RenderFrameHost* frame,
                      Observer* observer) override;

 private:
  std::unique_ptr<SerialChooserController> chooser_controller_;

  DISALLOW_COPY_AND_ASSIGN(ElectronSerialDelegate);
};

}  // namespace electron

#endif  // SHELL_BROWSER_SERIAL_ELECTRON_SERIAL_DELEGATE_H_
