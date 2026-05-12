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
    static std::string userSaveFileDialog(HWND owner, const std::string& defaultFileName, const std::string& extension, const std::string& description);
    static std::string userGetFileDialog(HWND owner, const std::vector<std::string>& extensions, const char* description);
    static int userPickRange(HWND hwndOwner, int L, int R, int initial, const std::string& name, const vector< short > &memoriezedData);
    static pair< Shape* , short > userReadPolygon(AppManager *appManager, const vector< short > &memoriezedData);
    static COLORREF userPickColor(HWND hwndOwner, const vector< short > &memoriezedData);
};



#endif
