# BrowseNX
**Launches the Internet Browser applet.**

## Credit
- p-sam - Supernag fix via sysmodule included in previous releases: https://github.com/p-sam/switch-sys-tweak AND wifiwebauthapplet fallback: https://github.com/switchbrew/libnx/commit/8360e561c5e48f7a2a704df3c97657e0d879629b used in previous releases
- XorTroll - Working out the browser applet arguments and for the nsp structure/build method.
- The-4n - Making legal NSP packing easy.
- Switchbrew - NPDMTool
## Building

### Requirements
 - Libnx
 - switch-tools devkitpro package

### Windows
 - You need to have devkitpro installed with msys, including command prompt support for msys-specific commands like `mv`
 - Run `make`

### Linux
 - Run `make`
