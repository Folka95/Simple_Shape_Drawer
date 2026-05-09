#pragma once
#ifndef INPUT_READER_H
#define INPUT_READER_H

inline COLORREF pickColor(HWND hwndOwner) {
    CHOOSECOLOR cc = {};
    static COLORREF customColors[16] = {0};

    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndOwner;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    cc.rgbResult = RGB(255, 255, 255); // optional initial color

    if (ChooseColor(&cc)) {
        return cc.rgbResult;
    }
    return RGB(0, 0, 0);
}

#endif //INPUT_READER_H
