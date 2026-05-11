#pragma once
#ifndef INPUT_READER_H
#define INPUT_READER_H
#include <windows.h>
#include <commctrl.h>
#include <string>
#pragma comment(lib, "comctl32.lib")

inline int pickRange(HWND hwndOwner, int L, int R, int initial, const std::string& name) {

    struct Data {
        int L, R, value;
        HWND slider;
        HWND label;
        HWND valueText;
        std::string title;
    };

    Data data;
    data.L = L;
    data.R = R;
    data.value = initial;
    data.title = name;

    const char* CLASS_NAME = "RangeSliderUI";

    WNDCLASS wc = {};
    wc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {

        Data* data = (Data*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        switch(msg) {

        case WM_CREATE: {

            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            data = (Data*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);

            std::string header =
                data->title + " (" +
                std::to_string(data->L) + " - " +
                std::to_string(data->R) + ")";

            data->label = CreateWindow("STATIC", header.c_str(),
                WS_VISIBLE | WS_CHILD,
                20, 10, 250, 20,
                hwnd, NULL, NULL, NULL
            );

            data->valueText = CreateWindow("STATIC", "",
                WS_VISIBLE | WS_CHILD,
                20, 35, 250, 20,
                hwnd, NULL, NULL, NULL
            );

            data->slider = CreateWindowEx(
                0, TRACKBAR_CLASS, "",
                WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                20, 60, 250, 40,
                hwnd, (HMENU)1, NULL, NULL
            );

            SendMessage(data->slider, TBM_SETRANGE, TRUE,
                MAKELPARAM(data->L, data->R));

            SendMessage(data->slider, TBM_SETPOS, TRUE, data->value);

            CreateWindow("BUTTON", "OK",
                WS_CHILD | WS_VISIBLE,
                110, 110, 60, 25,
                hwnd, (HMENU)2, NULL, NULL
            );

            // initial text
            std::string v = "Value: " + std::to_string(data->value);
            SetWindowText(data->valueText, v.c_str());

            break;
        }

        case WM_HSCROLL: {

            if (data && (HWND)lParam == data->slider) {

                data->value = SendMessage(data->slider, TBM_GETPOS, 0, 0);

                std::string v = "Value: " + std::to_string(data->value);
                SetWindowText(data->valueText, v.c_str());
            }

            break;
        }

        case WM_COMMAND: {

            if (LOWORD(wParam) == 2) {
                data->value = SendMessage(data->slider, TBM_GETPOS, 0, 0);
                DestroyWindow(hwnd);
            }

            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    };

    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Pick Value",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 180,
        hwndOwner, NULL, GetModuleHandle(NULL), &data
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return data.value;
}

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
