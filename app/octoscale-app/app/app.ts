/*
In NativeScript, the app.ts file is the entry point to your application.
You can use this file to perform app-level initialization, but the primary
purpose of the file is to pass control to the app’s first page.
*/

import { CanvasView } from '@nativescript-community/ui-canvas';
import { svelteNativeNoFrame } from 'svelte-native'
import { registerNativeViewElement } from 'svelte-native/dom'
import App from './App.svelte'

registerNativeViewElement("canvas", () => CanvasView);

svelteNativeNoFrame(App, {})
