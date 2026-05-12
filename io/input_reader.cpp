#include "input_reader.h"

std::string InputReader::userGetFileDialog(
    HWND owner,
    const std::vector<std::string>& extensions,
    const char* description
    ) {
    char fileName[MAX_PATH] = "";

    char filter[1024] = "";

    // Example:
    // "My Files (*.txt;*.png)\0*.txt;*.png\0"

    std::string patterns;

    for (int i = 0; i < extensions.size(); i++) {
        if (i)
            patterns += ";";

        patterns += "*." + extensions[i];
    }

    sprintf(
        filter,
        "%s (%s)%c%s%c",
        description,
        patterns.c_str(),
        '\0',
        patterns.c_str(),
        '\0'
    );

    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = filter;

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return fileName;
    }

    return "";
}

std::string InputReader::userSaveFileDialog(
    HWND owner,
    const std::string& defaultFileName,
    const std::string& extension,
    const std::string& description) {
    char fileName[MAX_PATH];

    strcpy(fileName, defaultFileName.c_str());

    char filter[256];

    // Example:
    // "Text Files (*.txt)\0*.txt\0"
    sprintf(
        filter,
        "%s (*.%s)%c*.%s%c",
        description.c_str(),
        extension.c_str(),
        '\0',
        extension.c_str(),
        '\0'
    );

    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;

    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = filter;

    ofn.lpstrDefExt = extension.c_str();

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        return fileName;
    }

    return "";
}

int InputReader::userPickRange(HWND hwndOwner, int L, int R, int initial, const std::string& name, const vector< short > &memoriezedData) {
    if(memoriezedData.size() == 1) {
        return memoriezedData[0];
    }
    if(memoriezedData.size() != 0) {
        cerr << "InputReader::userPickRange: invalid data size" << endl;
    }

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
                WS_CHILD | WS_VISIBLE,
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

    int rangeWSize = SCREEN_WIDTH*0.166666667;
    int rangeHSize = SCREEN_HEIGHT*0.166666667;

    int posx = (SCREEN_WIDTH - 320)/2;
    int posy = (SCREEN_HEIGHT - 180)/2;

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Pick Value",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        posx,posy ,rangeWSize , rangeHSize,
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

pair< Shape* , short > InputReader::userReadPolygon(
    AppManager *appManager,
    const vector< short > &memoriezedData
) {
    const int sz = userPickRange(
        appManager->getScreenOwner(),
        3,
        15,
        5,
        "Polygon Size",
        memoriezedData
    );

    switch (sz) {
        case 3:
            return {new PolygonShape<3>(), 3};

        case 4:
            return {new PolygonShape<4>(), 4};

        case 5:
            return {new PolygonShape<5>(), 5};

        case 6:
            return {new PolygonShape<6>(), 6};

        case 7:
            return {new PolygonShape<7>(), 7};

        case 8:
            return {new PolygonShape<8>(), 8};

        case 9:
            return {new PolygonShape<9>(), 9};

        case 10:
            return {new PolygonShape<10>(), 10};

        case 11:
            return {new PolygonShape<11>(), 11};

        case 12:
            return {new PolygonShape<12>(), 12};

        case 13:
            return {new PolygonShape<13>(), 13};

        case 14:
            return {new PolygonShape<14>(), 14};

        case 15:
            return {new PolygonShape<15>(), 15};

        default:
            return {nullptr, 0};
    }
}

COLORREF InputReader::userPickColor(HWND hwndOwner, const vector< short > &memoriezedData) {
    if(memoriezedData.size() == 3) {
        return RGB(memoriezedData[0], memoriezedData[1], memoriezedData[2]);
    }
    if(memoriezedData.size() != 0) {
        cerr << "InputReader::userPickColor: invalid data size" << endl;
    }
    CHOOSECOLOR cc = {};
    COLORREF customColors[16] = {0};
    COLORREF lastUsedColor = RGB(255, 255, 255);
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndOwner;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    cc.rgbResult = lastUsedColor;
    if (ChooseColor(&cc)) {
        lastUsedColor = cc.rgbResult;
        return cc.rgbResult;
    }
    return lastUsedColor;
}
