// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "shell/browser/serial/serial_chooser_context.h"

#include <utility>

#include "base/base64.h"
#include "base/memory/singleton.h"
#include "content/public/browser/device_service.h"
#include "mojo/public/cpp/bindings/pending_remote.h"

namespace electron {

SerialChooserContext* SerialChooserContext::GetInstance() {
  return base::Singleton<SerialChooserContext>::get();
}

SerialChooserContext::SerialChooserContext() = default;
SerialChooserContext::~SerialChooserContext() = default;

device::mojom::SerialPortManager* SerialChooserContext::GetPortManager() {
  EnsurePortManagerConnection();
  return port_manager_.get();
}

void SerialChooserContext::AddPortObserver(PortObserver* observer) {
  port_observer_list_.AddObserver(observer);
}

void SerialChooserContext::RemovePortObserver(PortObserver* observer) {
  port_observer_list_.RemoveObserver(observer);
}

base::WeakPtr<SerialChooserContext> SerialChooserContext::AsWeakPtr() {
  return weak_factory_.GetWeakPtr();
}

void SerialChooserContext::OnPortAdded(device::mojom::SerialPortInfoPtr port) {
  for (auto& observer : port_observer_list_)
    observer.OnPortAdded(*port);
}

void SerialChooserContext::OnPortRemoved(
    device::mojom::SerialPortInfoPtr port) {
  for (auto& observer : port_observer_list_)
    observer.OnPortRemoved(*port);
}

void SerialChooserContext::EnsurePortManagerConnection() {
  if (port_manager_)
    return;

  mojo::PendingRemote<device::mojom::SerialPortManager> manager;
  content::GetDeviceService().BindSerialPortManager(
      manager.InitWithNewPipeAndPassReceiver());
  SetUpPortManagerConnection(std::move(manager));
}

void SerialChooserContext::SetUpPortManagerConnection(
    mojo::PendingRemote<device::mojom::SerialPortManager> manager) {
  port_manager_.Bind(std::move(manager));
  port_manager_.set_disconnect_handler(
      base::BindOnce(&SerialChooserContext::OnPortManagerConnectionError,
                     base::Unretained(this)));

  port_manager_->SetClient(client_receiver_.BindNewPipeAndPassRemote());
}

void SerialChooserContext::OnPortManagerConnectionError() {
  port_manager_.reset();
  client_receiver_.reset();
}

}  // namespace electron
