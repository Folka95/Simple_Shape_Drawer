#pragma once
#ifndef SCREEN_WRITER_H
#define SCREEN_WRITER_H
#include <windows.h>
#include <vector>
using namespace std;

class ScreenWriter {
private:
    HDC hdc;
    HWND hwnd;
    COLORREF backgroundColor;
    bool isActive;
    vector< vector< bool > > isUserDrawn;
    vector< vector< COLORREF > > screen;
public:
    ScreenWriter(HWND hwnd);
    ~ScreenWriter();
    void setBackgroundColor(COLORREF color);
    void changeBackgroundColor(COLORREF color);
    void setPixel(int x, int y, COLORREF color);
    COLORREF getPixel(int x, int y);
    void clearScreen();
    int getHeight();
    int getWidth();
    bool outOfBounds(int x, int y);
    void activate();
    void deactivate();
};

#endif //SCREEN_WRITER_H
