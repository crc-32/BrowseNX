// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// Main program entrypoint
int main(int argc, char* argv[])
{
    Result rc=0;
    int i = 0;
    char url[32] = {0};
    AppletHolder aHold;
    AppletStorage aStore;
    LibAppletArgs aArgs;
    SwkbdConfig kbd;
    consoleInit(NULL);
    strcpy(url, "https://dns.switchbru.com");
    printf("Press L to choose url");
    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
        if(kDown & KEY_L) {
            char tmpoutstr[32] = {0};
            rc = swkbdCreate(&kbd, 0);
            if(R_SUCCEEDED(rc)) {
                swkbdConfigMakePresetDefault(&kbd);
                swkbdConfigSetOkButtonText(&kbd, "Go");
                swkbdConfigSetGuideText(&kbd, "Enter a URL");
                swkbdConfigSetInitialText(&kbd, "https://");
                rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
                if(R_SUCCEEDED(rc)) {
                    if(tmpoutstr != 0) {
                        strcpy(url, tmpoutstr);
                    }
                }else if(rc != (Result) 0x5d59) {
                    printf("Error with URL");
                    consoleUpdate(NULL);
                    while(1){
                        svcSleepThread(100000);
                    }
                }
            }else{
                fatalSimple(rc);
            }
            swkbdClose(&kbd);
            break;
        }
        i++;
        if(i >= 60*2){
            break;
        }
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    appletCreateLibraryApplet(&aHold, AppletId_web, LibAppletMode_AllForeground);
    libappletArgsCreate(&aArgs, 0x50000);
    libappletArgsPush(&aArgs, &aHold);
    appletCreateStorage(&aStore, 8192);

    u8 indata[8192] = {0};
    *(u64*)&indata[4] = 281530811285509;
    *(u64*)&indata[17] = 201326593;
    *(u8*)&indata[16] = 1;
    *(u16*)indata = 2;
    strcpy((char*)&indata[25], url);

    appletStorageWrite(&aStore, 0, indata, 8192);
    appletHolderPushInData(&aHold, &aStore);
    appletHolderStart(&aHold);
    appletHolderJoin(&aHold);
    appletHolderClose(&aHold);
    appletStorageClose(&aStore);
    return 0;
}
