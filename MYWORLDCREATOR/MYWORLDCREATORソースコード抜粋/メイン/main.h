#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <functional>
#include<string>
#include <algorithm>

#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_win32.h"
#include "imgui\\imgui_impl_dx11.h"

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define SCENE_WIDTH_L (150)
#define SCENE_WIDTH_R (980)
#define SCENE_HEIGHT (SCREEN_HEIGHT)

#define MS_PREASS_UP 0
#define MS_PREASS_DOWN 1
#define MS_TRIGGER_UP 2
#define MS_TRIGGER_DOWN 3
#define MS_SKIP 4

struct MOUSESTATE
{
	int LBUTTON = MS_PREASS_UP;
	int MBUTTON = MS_PREASS_UP;
	int RBUTTON = MS_PREASS_UP;
	
};


HWND GetWindow();
POINT GetMousePos();
POINT GetMouseDelta();
const MOUSESTATE GetMouseState();
void SetMouseSkip();
bool GetMouseSkip();
void MouseProcess();