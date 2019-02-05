// TODO: Remove and use libnx impl once its in a stable release
#pragma once
#include <switch.h>

typedef struct {
    u8 unk_x0[0x4];
    char url[0xFC];
} WebWifiPageArgUrl;

typedef struct {
    WebWifiPageArgUrl url1;
    WebWifiPageArgUrl url2;
    u8 unk_x200[0x300];
    u8 unk_x500[0x18];
} WebWifiPageArg;

typedef struct {
    WebWifiPageArg arg;
} WebWifiConfig;

void webWifiCreate(WebWifiConfig* config, const char* url);
Result webWifiShow(WebWifiConfig* config);