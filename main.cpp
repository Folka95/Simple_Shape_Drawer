#pragma once
#include <bits/stdc++.h>
#include <Windows.h>
#include "rendering/menu_creator.h"
#include "io/input_reader.h"
#include "app/app_manager.h"
#include "core/shapes/line.h"
#include "core/shapes/circle.h"
#include "core/shapes/rectangle.h"
#include "algorithms/line/line_dda_drawing_algorithm.h"
#include "algorithms/circle/circle_Polar_drawing_algorithm.h"
#include "algorithms/circle/circle_iterativePolar_drawing_algorithm.h"
#include "algorithms/clipping/circle/circle_line_clipping_algorithm.h"
#include "algorithms/filling/IterFloodFill_filling_algorithm.h"

using namespace std;

AppManager *appManager;

void menuSelected(WPARAM &wp, LPARAM &lp) {
    switch (LOWORD(wp)) {

        // ================= FILE =================
        case FILE_CLEAR:
            appManager->clearScreen();
            break;

        case FILE_SAVE:
            appManager->saveScreen();
            break;

        case FILE_LOAD:
            appManager->loadScreen();
            break;

        // ================= PREFERENCES =================
        case PREFERENCES_BACKGROUND_COLOR:
            appManager->setBackgroundColor(pickColor());
            break;

        case PREFERENCES_MOUSE_SHAPE:
            break;

        case PREFERENCES_BORDER_COLOR:
            appManager->setBoarderColor(pickColor());
            break;

        case PREFERENCES_FILL_COLOR:
            appManager->setFillColor(pickColor());
            break;

        // ================= LINE =================
        case LINE_DDA:
            appManager->setShape(new Line());
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setDrawingAlgorithm(new Line_DDA_DrawingAlgorithm());
            break;
        //
        // case LINE_MIDPOINT:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // case LINE_PARAMETRIC:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // // ================= CIRCLE =================
        // case CIRCLE_Direct:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        case CIRCLE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_Polar_DrawingAlgorithm());
            break;
        //
        case CIRCLE_ITERATIVE_Polar:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_iterativePolar_DrawingAlgorithm());
            break;
        //
        // case CIRCLE_MIDPOINT:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // case CIRCLE_MODIFIED_MIDPOINT:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // // ================= ELLIPSE =================
        // case ELLIPSE_DIRECT:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // case ELLIPSE_POLAR:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // case ELLIPSE_MIDPOINT:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // // ================= CURVES =================
        // case CURVE_CARDINAL_SPLINE:
        //     appManager->setDrawingAlgorithm();
        //     break;
        //
        // // ================= FILLING =================
        // case FILL_CIRCLE_LINES:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_CIRCLE_CIRCLES:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_SQUARE_HERMITE:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_RECTANGLE_BEZIER:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_CONVEX:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_NON_CONVEX:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        // case FILL_FLOOD_RECURSIVE:
        //     appManager->setFillingAlgorithm();
        //     break;
        //
        case FILL_FLOOD_NON_RECURSIVE:
            appManager->setFillingAlgorithm(new IterFloodFill_FillingAlgorithm());
            break;

        // // ================= RECT CLIP =================
        // case CLIP_RECT_POINT:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_RECT_LINE:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_RECT_POLYGON:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // // ================= SQUARE CLIP =================
        // case CLIP_SQUARE_POINT:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_SQUARE_LINE:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // // ================= CIRCLE CLIP =================
        // case CLIP_CIRCLE_POINT:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        case CLIP_CIRCLE_LINE:
            appManager->removeDrawingAlgorithm();
            appManager->setClippingAlgorithm(
                new Circle_line_ClippingAlgorithm(),
                new Circle_Polar_DrawingAlgorithm(),
                new Circle()
            );
            appManager->setShape(new Line());
            break;
        //
        // // ================= SMILE =================
        // case SMILE_HAPPY_FACE:
        //     appManager->setShape();
        //     break;
        //
        // case SMILE_SAD_FACE:
        //     appManager->setShape();
        //     break;

        default:
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