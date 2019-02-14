// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "web_wifi.h"

void showError(char* errorText1, char* errorText2, Result outrc)
{
    AppletHolder err;
    AppletStorage errStor;
    LibAppletArgs errArgs;

    appletCreateLibraryApplet(&err, AppletId_error, LibAppletMode_AllForeground);
    libappletArgsCreate(&errArgs, 1);
    libappletArgsPush(&errArgs, &err);
    appletCreateStorage(&errStor, 4120);
    u8 args[4120] = {0};
    args[0] = 1;

    u64 e = (((outrc & 0x1ffu) + 2000) | (((outrc >> 9) & 0x1fff & 0x1fffll) << 32));
    *(u64*)&args[8] = e;
    strcpy((char*) &args[24], errorText1);
    strcpy((char*) &args[2072], errorText2);
    appletStorageWrite(&errStor, 0, args, 4120);
    appletHolderPushInData(&err, &errStor);

    appletHolderStart(&err);
    appletHolderJoin(&err);
    exit(1);
}

SwkbdTextCheckResult validate_text(char* tmp_string, size_t tmp_string_size) {
    if (strncmp(tmp_string, "https://", 8) !=0 && strncmp(tmp_string, "http://", 7) !=0) {
        strncpy(tmp_string, "Add 'https://' or 'http://' to url", tmp_string_size);
        return SwkbdTextCheckResult_Bad;
    }
    if(strlen(tmp_string) < 12) {
        strncpy(tmp_string, "URL too short to be valid", tmp_string_size);
        return SwkbdTextCheckResult_Bad;
    }
    static const char *allowed=":/.%0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *reset = tmp_string;
    while(*tmp_string)
    {
        if(strchr(allowed, *tmp_string) == NULL){
            strncpy(reset, "Remove special characters from URL", tmp_string_size);
            return SwkbdTextCheckResult_Bad;
        }
        tmp_string++;
    }

    return SwkbdTextCheckResult_OK;
}

int showKeyboard(char out[0xc00], char* title, char* placehold, char* oktxt, char* initial)
{
    int rc;
    SwkbdConfig kbd;
    char tmpoutstr[0xc00] = {0};
    rc = swkbdCreate(&kbd, 0);
    if(R_SUCCEEDED(rc)) {
        swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetHeaderText(&kbd, title);
        swkbdConfigSetOkButtonText(&kbd, oktxt);
        swkbdConfigSetGuideText(&kbd, placehold);
        swkbdConfigSetInitialText(&kbd, initial);
        swkbdConfigSetTextCheckCallback(&kbd, validate_text);
        rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
        if(R_SUCCEEDED(rc)) {
            if(tmpoutstr[0] != '\0') {
                strcpy(out, tmpoutstr);
                swkbdClose(&kbd);
                return 0;
            }
        }else if(rc != (Result) 0x5d59) {
            showError("Software Keyboard Error", "Unknown error, Lookup errorcode for more info", rc);
        }
    }else{
        showError("Software Keyboard Error", "Unknown error, Lookup errorcode for more info", rc);
    }
    swkbdClose(&kbd);
    return -1;
}
void startAuthApplet(char* url) {
    Result rc=0;
    // TODO: Move to libnx impl once its in a stable release
    WebWifiConfig config;
    webWifiCreate(&config, url);
    rc = webWifiShow(&config);
    if(R_FAILED(rc)) {
        showError("Browser Error", "Lookup errorcode for more info", rc);
    }
}

int main(int argc, char* argv[])
{
    void *haddr;
    if(!envIsNso()){
        // Heap trick to give some memory to the applet pool, otherwise won't work from album
        // FIXME: Album still doesn't work
        // svcSetHeapSize(&haddr, 0x13001000);
    }
    Result rc=0;
    int i = 0;
    char url[0xc00] = {0};
    AppletHolder aHold;
    AppletStorage aStore;
    LibAppletArgs aArgs;
    nsvmInitialize();
    pminfoInitialize();
    consoleInit(NULL);
    strcpy(url, "https://dns.switchbru.com");
    printf("Press [L] to choose url\n");
    printf("Press [R] to set default url\n");
    printf("Press [X] to reset default url\n");
    if(!envIsNso()){
        printf(CONSOLE_RED "NRO Mode, launching limited browser!\n");
    }else {
        printf("Press [-] to launch as auth applet " CONSOLE_RED "(LIMITED FEATURES)");
    }
    bool nagOn;
    nsvmNeedsUpdateVulnerability(&nagOn);
    if(nagOn) {
        u32 pCount;
        u64 pids[100];
        u64 cId;
        u32 i = 0;
        bool isPatched = false;
        svcGetProcessList(&pCount, pids, 100);
        while (i <= pCount-1) {
            pminfoGetTitleId(&cId, pids[i]);
            if(cId == 0x00FF747765616BFF || cId == 0x01FF415446660000) {
                printf(CONSOLE_GREEN "Supernag enabled, but patched via switch-sys-tweak!\n");
                isPatched = true;
                break;
            }
            i++;
        }
        if(!isPatched){
            showError("Error: Nag active, check more details", "Browser won't launch if supernag is active\n\nUse gagorder or switch-sys-tweak (the latter is bundled with BrowseNX) to disable supernag.", 0);
        }
    }
    bool forceAuth = false;
    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
        if(kDown & KEY_L) {
            showKeyboard(url, "Choose URL", "Enter a URL", "Go", "https://");
            break;
        } else if(kDown & KEY_R) {
            char defUrl[0xc00] = {0};
            char fURL[0xc00] = {0};
            FILE* dUCheck = fopen("sdmc:/defUrl.txt", "r");
            if (dUCheck != NULL) {
                fgets(fURL, 0xc00, dUCheck);
            }else {
                strcpy(fURL, url);
            }
            fclose(dUCheck);
            if(showKeyboard(defUrl, "Set default URL", "Enter A URL", "Set", fURL) == 0) {
                FILE* dUFile = fopen("sdmc:/defUrl.txt", "w+");
                fprintf(dUFile, "%s", defUrl);
                fclose(dUFile);
            }
        } else if(kDown & KEY_X) {
            remove("sdmc:/defUrl.txt");
        } else if(kDown & KEY_MINUS) {
            forceAuth = true;
        }
        i++;
        if(i >= 60*2){
            break;
        }
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    pminfoExit();
    nsvmExit();

    FILE* dUFile = fopen("sdmc:/defUrl.txt", "r");
    char fURL[0xc00] = {0};
    if(dUFile != NULL) {
        fgets(fURL, 0xc00, dUFile);
        fclose(dUFile);
        if(validate_text(fURL, 0xc00) == SwkbdTextCheckResult_OK) {
            strcpy(url, fURL);
        }else{
            showError("Default URL file error, check more details", fURL, 0);
        }
    }
    if(envIsNso() && !forceAuth) { // Running as a title
        rc = appletCreateLibraryApplet(&aHold, AppletId_web, LibAppletMode_AllForeground);
        if(R_FAILED(rc)) {
            showError("Error launching browser", "Error initializing applet", rc);
        }
        libappletArgsCreate(&aArgs, 0x50000);
        libappletArgsPush(&aArgs, &aHold);
        rc = appletCreateStorage(&aStore, 8192);
        if(R_FAILED(rc)) {
            showError("Error launching browser", "Error initializing arg storage", rc);
        }

        u8 indata[8192] = {0};
        *(u64*)&indata[4] = 281530811285509;
        *(u64*)&indata[17] = 201326593;
        *(u8*)&indata[16] = 1;
        *(u16*)indata = 2;
        strcpy((char*)&indata[25], url);

        rc = appletStorageWrite(&aStore, 0, indata, 8192);
        if(R_FAILED(rc)) {
            showError("Error launching browser", "Error writing arg storage", rc);
        }
        appletHolderPushInData(&aHold, &aStore);
        rc = appletHolderStart(&aHold);
        if(R_FAILED(rc)) {
            showError("Error launching browser", "Lookup errorcode for more info", rc);
        }
        appletHolderJoin(&aHold);
        LibAppletExitReason e = appletHolderGetExitReason(&aHold);
        if(e != LibAppletExitReason_Normal) {
            showError("Browser Error", "Lookup errorcode for more info", rc);
        }
        appletHolderClose(&aHold);
        appletStorageClose(&aStore);
    } else { // Running under HBL
        startAuthApplet(url);
    }
    return 0;
}
