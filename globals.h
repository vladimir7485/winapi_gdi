#ifndef _GLOBALS_
#define _GLOBALS_

#pragma warning(disable:4995)

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <objbase.h>

#define pi 3.14
#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)
#define MAPWIDTH 1024
#define MAPHEIGHT 768

enum CURVE_ID : int {PARABOLA_ID=0, GYPERBOLA_ID, SINUS_ID, TANGENT_ID};

#endif