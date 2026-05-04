#ifndef SCREEN_WRITER_H
#define SCREEN_WRITER_H
#include <windows.h>

class ScreenWriter {
private:
    HDC *hdc;
    HWND hwnd;
    COLORREF backgroundColor;
public:
    ScreenWriter(HWND hwnd);
    ~ScreenWriter();
    void setBackgroundColor(COLORREF color);
    void setPixel(int x, int y, COLORREF color);
    void clearScreen();
    int getHeight();
    int getWidth();
    bool outOfBounds(int x, int y);
    void activate();
    void deactivate();
};

#endif //SCREEN_WRITER_H
