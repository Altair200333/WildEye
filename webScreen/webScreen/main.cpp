#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winhttp.lib")
#include <iostream>
#include <Windows.h>
#include "Commands.h"
#include <winhttp.h>
#include "HttpRequest.h"
#include <atlstr.h>
#include <fstream>
#include <chrono>
#include <map>
#include <thread>
#include "asc2art.h"
#include "debinaryData.h"
#include <Mmsystem.h>
#include <mciapi.h>

#pragma comment(lib, "Winmm.lib")
#include "ShadowPress.h"

int main()
{
    ShadowPress control;
    control.mainLoop();

    return 0;
}