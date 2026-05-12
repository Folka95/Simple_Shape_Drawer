#pragma once
#ifndef INPUT_READER_H
#define INPUT_READER_H
#include <windows.h>
#include "../core/shape.h"
#include "../core/shapes/polygon.h"
#include "../app/app_manager.h"
#include <commctrl.h>
#include <string>
#pragma comment(lib, "comctl32.lib")

const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

class InputReader {
public:
    static int userPickRange(HWND hwndOwner, int L, int R, int initial, const std::string& name);
    static Shape* userReadPolygon(AppManager *appManager);
    static COLORREF userPickColor(HWND hwndOwner);
};



#endif
