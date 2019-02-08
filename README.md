# NXBrowser
**Launches the Internet Browser applet within a NSP.**
_To build BrowseNX, please supply your own prod.keys._
## Credit
- p-sam - Supernag fix via sysmodule included in each release: https://github.com/p-sam/switch-sys-tweak AND wifiwebauthapplet fallback: https://github.com/switchbrew/libnx/commit/8360e561c5e48f7a2a704df3c97657e0d879629b not yet used in a libnx release
- XorTroll - Working out the browser applet arguments and for the nsp structure/build method.
- The-4n - Making legal NSP packing easy.
- Switchbrew - NPDMTool
## Building

### Requirements
 - Libnx
 - switch-tools devkitpro package
 - prod.keys file at the repo root

### Windows
 - You need to have devkitpro installed with msys, including command prompt support for msys-specific commands like `mv`
 - Run `buildwin.bat`

### Linux
 - Run `build.sh`
