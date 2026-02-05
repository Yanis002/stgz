extern void FS_LoadOverlay(int param1, int overlayID);
extern void RTC_Init();

// replace the `RTC_Init` call from `func_ov018_020c4e8c` so we can load the gz overlay
void RTCInit_hook() {
    FS_LoadOverlay(0, 114);
    RTC_Init();
}
