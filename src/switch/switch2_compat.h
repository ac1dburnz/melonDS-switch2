// Compatibility for the pre-libnx-4 input API used by this frontend.
#pragma once
#include <switch.h>
#define CONTROLLER_P1_AUTO 0
#define KEY_TOUCH BIT(28)
#define KEY_A HidNpadButton_A
#define KEY_B HidNpadButton_B
#define KEY_X HidNpadButton_X
#define KEY_Y HidNpadButton_Y
#define KEY_L HidNpadButton_L
#define KEY_R HidNpadButton_R
#define KEY_ZL HidNpadButton_ZL
#define KEY_ZR HidNpadButton_ZR
#define KEY_PLUS HidNpadButton_Plus
#define KEY_MINUS HidNpadButton_Minus
#define KEY_LSTICK HidNpadButton_StickL
#define KEY_RSTICK HidNpadButton_StickR
#define KEY_UP HidNpadButton_AnyUp
#define KEY_DOWN HidNpadButton_AnyDown
#define KEY_LEFT HidNpadButton_AnyLeft
#define KEY_RIGHT HidNpadButton_AnyRight
#define KEY_RSTICK_DOWN HidNpadButton_StickRDown
struct touchPosition { u32 px, py; };
void hidScanInput();
u32 hidKeysDown(int);
u32 hidKeysUp(int);
u32 hidKeysHeld(int);
u32 hidTouchCount();
void hidTouchRead(touchPosition*, int);

#define KEY_DUP HidNpadButton_Up
#define KEY_DDOWN HidNpadButton_Down
#define KEY_DLEFT HidNpadButton_Left
#define KEY_DRIGHT HidNpadButton_Right
#define KEY_LSTICK_UP HidNpadButton_StickLUp
#define KEY_LSTICK_DOWN HidNpadButton_StickLDown
#define KEY_LSTICK_LEFT HidNpadButton_StickLLeft
#define KEY_LSTICK_RIGHT HidNpadButton_StickLRight
#define KEY_RSTICK_UP HidNpadButton_StickRUp
#define KEY_RSTICK_LEFT HidNpadButton_StickRLeft
#define KEY_RSTICK_RIGHT HidNpadButton_StickRRight
