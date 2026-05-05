#pragma once
#ifndef INPUT_READER_H
#define INPUT_READER_H
#include <windows.h>

COLORREF pickColor() {
    CHOOSECOLOR cc = {};
    static COLORREF customColors[16] = {0};
    HWND hwnd;
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        return cc.rgbResult;
    }

    return RGB(0,0,0);
}

#endif //INPUT_READER_H
