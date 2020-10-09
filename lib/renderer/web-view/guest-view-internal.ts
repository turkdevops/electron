import { webFrame, IpcMessageEvent } from 'electron';
import { ipcRendererInternal } from '@electron/internal/renderer/ipc-renderer-internal';
import * as ipcRendererUtils from '@electron/internal/renderer/ipc-renderer-internal-utils';
import { webViewEvents } from '@electron/internal/common/web-view-events';

import { WebViewImpl } from '@electron/internal/renderer/web-view/web-view-impl';

const DEPRECATED_EVENTS: Record<string, string> = {
  'page-title-updated': 'page-title-set'
};

const dispatchEvent = function (
  webView: WebViewImpl, eventName: string, eventKey: string, ...args: Array<any>
) {
  if (DEPRECATED_EVENTS[eventName] != null) {
    dispatchEvent(webView, DEPRECATED_EVENTS[eventName], eventKey, ...args);
  }

  const domEvent = new Event(eventName) as ElectronInternal.WebViewEvent;
  webViewEvents[eventKey].forEach((prop, index) => {
    (domEvent as any)[prop] = args[index];
  });

  webView.dispatchEvent(domEvent);

  if (eventName === 'load-commit') {
    webView.onLoadCommit(domEvent);
  } else if (eventName === 'focus-change') {
    webView.onFocusChange();
  }
};

export function registerEvents (webView: WebViewImpl, viewInstanceId: number) {
  ipcRendererInternal.on(`ELECTRON_GUEST_VIEW_INTERNAL_DESTROY_GUEST-${viewInstanceId}`, function () {
    webView.guestInstanceId = undefined;
    webView.reset();
    const domEvent = new Event('destroyed');
    webView.dispatchEvent(domEvent);
  });

  ipcRendererInternal.on(`ELECTRON_GUEST_VIEW_INTERNAL_DISPATCH_EVENT-${viewInstanceId}`, function (event, eventName, ...args) {
    dispatchEvent(webView, eventName, eventName, ...args);
  });

  ipcRendererInternal.on(`ELECTRON_GUEST_VIEW_INTERNAL_IPC_MESSAGE-${viewInstanceId}`, function (event, channel, ...args) {
    const domEvent = new Event('ipc-message') as IpcMessageEvent;
    domEvent.channel = channel;
    domEvent.args = args;

    webView.dispatchEvent(domEvent);
  });
}

export function deregisterEvents (viewInstanceId: number) {
  ipcRendererInternal.removeAllListeners(`ELECTRON_GUEST_VIEW_INTERNAL_DESTROY_GUEST-${viewInstanceId}`);
  ipcRendererInternal.removeAllListeners(`ELECTRON_GUEST_VIEW_INTERNAL_DISPATCH_EVENT-${viewInstanceId}`);
  ipcRendererInternal.removeAllListeners(`ELECTRON_GUEST_VIEW_INTERNAL_IPC_MESSAGE-${viewInstanceId}`);
}

export function createGuest (params: Record<string, any>): Promise<number> {
  return ipcRendererInternal.invoke('ELECTRON_GUEST_VIEW_MANAGER_CREATE_GUEST', params);
}

export function attachGuest (
  elementInstanceId: number, guestInstanceId: number, params: Record<string, any>, contentWindow: Window
) {
  const embedderFrameId = webFrame.getWebFrameId(contentWindow);
  if (embedderFrameId < 0) { // this error should not happen.
    throw new Error('Invalid embedder frame');
  }
  ipcRendererInternal.invoke('ELECTRON_GUEST_VIEW_MANAGER_ATTACH_GUEST', embedderFrameId, elementInstanceId, guestInstanceId, params);
}

export function detachGuest (guestInstanceId: number) {
  return ipcRendererUtils.invokeSync('ELECTRON_GUEST_VIEW_MANAGER_DETACH_GUEST', guestInstanceId);
}

export const guestViewInternalModule = {
  deregisterEvents,
  createGuest,
  attachGuest,
  detachGuest
};
