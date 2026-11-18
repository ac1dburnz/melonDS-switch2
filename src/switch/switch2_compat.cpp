#include "switch2_compat.h"
namespace {
PadState pad;
bool initialized = false;
HidTouchScreenState touch;
}
void hidScanInput() {
    if (!initialized) {
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        hidInitializeTouchScreen();
        initialized = true;
    }
    padUpdate(&pad);
    if (hidGetTouchScreenStates(&touch, 1) < 1) touch.count = 0;
}
u32 hidKeysDown(int) { return static_cast<u32>(padGetButtonsDown(&pad)); }
u32 hidKeysUp(int) { return static_cast<u32>(padGetButtonsUp(&pad)); }
u32 hidKeysHeld(int) { return static_cast<u32>(padGetButtons(&pad)); }
u32 hidTouchCount() { return touch.count; }
void hidTouchRead(touchPosition* out, int index) {
    *out = {};
    if (index >= 0 && index < touch.count) {
        out->px = touch.touches[index].x;
        out->py = touch.touches[index].y;
    }
}
