#pragma once
#include <bits/stdc++.h>
#include <Windows.h>
#include "rendering/menu_creator.h"
#include "app/app_manager.h"
using namespace std;

AppManager *appManager;

void menuSelected(WPARAM &wp, LPARAM &lp) {
    switch (LOWORD(wp)) {
        case FILE_CLEAR:
            appManager->clearScreen();
            break;
        case FILE_SAVE:
            break;
        case FILE_LOAD:
            break;
    }
}

void mouseRightClick(WPARAM wp, LPARAM lp) {
    int x = LOWORD(lp);
    int y = HIWORD(lp);
    appManager->applyRightClick(x, y);
}

void mouseLeftClick(WPARAM wp, LPARAM lp) {
    int x = LOWORD(lp);
    int y = HIWORD(lp);
    appManager->applyLeftClick(x, y);
}

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {
        case WM_COMMAND:
            menuSelected(wp, lp);
            break;
        case WM_RBUTTONDOWN:
            mouseRightClick(wp, lp);
            break;
        case WM_LBUTTONDOWN:
            mouseLeftClick(wp, lp);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE h, HINSTANCE, LPSTR, int nsh) {
    WNDCLASS wc = {};

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hInstance = h;
    wc.lpfnWndProc = WndProc;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "myclass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        "Simple Drawer",
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL, NULL, h, 0
    );

    HMENU hMenu = CreateMenu();
    createAppMenu(hMenu);

    SetMenu(hwnd, hMenu);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    appManager = new AppManager(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}