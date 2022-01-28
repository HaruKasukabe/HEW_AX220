//=============================================================================
//
// 入力処理 [input.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'

#define JOYBUTTON1	0 // Aボタン
#define JOYBUTTON2	1 // Bボタン
#define JOYBUTTON3	2 // Xボタン
#define JOYBUTTON4	3 // Yボタン
#define JOYBUTTON5	4
#define JOYBUTTON6	5
#define JOYBUTTON7	6
#define JOYBUTTON8	7 // STARTボタン
#define JOYBUTTON9	8
#define JOYBUTTON10	9
#define JOYBUTTON11	10
#define JOYBUTTON12	11
#define JOYBUTTON13	12
#define JOYBUTTON14	13
#define JOYBUTTON15	14
#define JOYBUTTON16	15
#define JOYBUTTON17	16
#define JOYBUTTON18	17
#define JOYBUTTON19	18
#define JOYBUTTON20	19
#define JOYBUTTON21	20
#define JOYBUTTON22	21
#define JOYBUTTON23	22
#define JOYBUTTON24	23
#define JOYBUTTON25	24
#define JOYBUTTON26	25
#define JOYBUTTON27	26
#define JOYBUTTON28	27
#define JOYBUTTON29	28
#define JOYBUTTON30	29
#define JOYBUTTON31	30
#define JOYBUTTON32	31

#define JOYSTICKID3		2
#define JOYSTICKID4		3
#define JOYSTICKID5		4
#define JOYSTICKID6		5
#define JOYSTICKID7		6
#define JOYSTICKID8		7
#define JOYSTICKID9		8
#define JOYSTICKID10	9
#define JOYSTICKID11	10
#define JOYSTICKID12	11
#define JOYSTICKID13	12
#define JOYSTICKID14	13
#define JOYSTICKID15	14
#define JOYSTICKID16	15

#define JOY_DPAD_CENTER	0
#define JOY_DPAD_DOWN	1
#define JOY_DPAD_RIGHT	2
#define JOY_DPAD_LEFT	4
#define JOY_DPAD_UP		8

#define MOUSEBUTTON_L	0
#define MOUSEBUTTON_R	1
#define MOUSEBUTTON_M	2
#define MOUSEBUTTON_X1	3
#define MOUSEBUTTON_X2	4

#ifndef INPUT_DEADZONE
#define INPUT_DEADZONE	(0.24f*float(0x7FFF))	// Default to 24% of the +/- 32767 range. This is a reasonable default value but can be altered if needed.
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput();
void UninitInput();
void UpdateInput();

bool GetKeyPress(int nKey);
bool GetKeyTrigger(int nKey);
bool GetKeyRelease(int nKey);
bool GetKeyRepeat(int nKey);

DWORD GetJoyCount();
JOYINFOEX *GetJoyState(DWORD dwJoy);
LONG GetJoyX(DWORD dwJoy);
LONG GetJoyY(DWORD dwJoy);
LONG GetJoyZ(DWORD dwJoy);
LONG GetJoyR(DWORD dwJoy);
LONG GetJoyU(DWORD dwJoy);
LONG GetJoyV(DWORD dwJoy);
float GetJoyfX(DWORD dwJoy);
float GetJoyfY(DWORD dwJoy);
float GetJoyfZ(DWORD dwJoy);
float GetJoyfR(DWORD dwJoy);
float GetJoyfU(DWORD dwJoy);
float GetJoyfV(DWORD dwJoy);
bool GetJoyButton(DWORD dwJoy, DWORD dwBtn);
bool GetJoyTrigger(DWORD dwJoy, DWORD dwBtn);
bool GetJoyRelease(DWORD dwJoy, DWORD dwBtn);
DWORD GetJoyDpad(DWORD dwJoy);
bool GetJoyDpadUp(DWORD dwJoy);
bool GetJoyDpadLeft(DWORD dwJoy);
bool GetJoyDpadRight(DWORD dwJoy);
bool GetJoyDpadDown(DWORD dwJoy);

POINT *GetMousePosition();
bool GetMouseButton(DWORD dwBtn);
bool GetMouseTrigger(DWORD dwBtn);
bool GetMouseRelease(DWORD dwBtn);
