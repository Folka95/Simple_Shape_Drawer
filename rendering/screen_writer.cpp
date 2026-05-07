#include "screen_writer.h"

#include <iostream>
#include <ostream>

ScreenWriter::ScreenWriter(HWND hwnd) {
    isActive = false;
    this->hwnd = hwnd;
    backgroundColor = RGB(0, 0, 0);
    isUserDrawn = vector< vector< bool > > (this->getWidth(), vector< bool >(this->getHeight(), false));
    screen = vector< vector< COLORREF > > (this->getWidth(), vector< COLORREF >(this->getHeight(), backgroundColor));
}

ScreenWriter::~ScreenWriter() {
    this->deactivate();
}

void ScreenWriter::setBackgroundColor(COLORREF color) {
    backgroundColor = color;
}

void ScreenWriter::changeBackgroundColor(COLORREF color) {
    setBackgroundColor(color);
    HDC hdc = GetDC(hwnd);
    for(int i = 0; i < this->getWidth(); i++) {
        for(int j = 0; j < this->getHeight(); j++) {
            if(!isUserDrawn[i][j]) {
                screen[i][j] = backgroundColor;
                SetPixel(hdc, i, j, backgroundColor);
            }
        }
    }
    ReleaseDC(this->hwnd, hdc);
}

void ScreenWriter::setPixel(int x, int y, COLORREF color) {
    if (!isActive) {
        std::cerr << "ScreenWriter::setPixel: sw is not activated" << std::endl;
        return;
    }
    if (this->outOfBounds(x, y)) {
        std::cerr << "ScreenWriter::setPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return;
    }
    SetPixel(this->hdc, x, y, color);
    screen[x][y] = color;
    isUserDrawn[x][y] = true;
}

COLORREF ScreenWriter::getPixel(int x, int y) {
    if (this->outOfBounds(x, y)) {
        std::cerr << "ScreenWriter::setPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return backgroundColor;
    }
    return screen[x][y];
}

void ScreenWriter::clearScreen() {
    HDC hdc = GetDC(hwnd);

    RECT rect;
    GetClientRect(this->hwnd, &rect);

    HBRUSH brush = CreateSolidBrush(backgroundColor);
    FillRect(hdc, &rect, brush);

    DeleteObject(brush);
    ReleaseDC(this->hwnd, hdc);
    isUserDrawn = vector< vector< bool > > (this->getWidth(), vector< bool >(this->getHeight(), false));
    screen = vector< vector< COLORREF > > (this->getWidth(), vector< COLORREF >(this->getHeight(), backgroundColor));
}

int ScreenWriter::getWidth() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    return rect.right;
}

int ScreenWriter::getHeight() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    return rect.bottom;
}

bool ScreenWriter::outOfBounds(int x, int y) {
    return (x < 0 || y < 0 || x >= getWidth() || y >= getHeight());
}

void ScreenWriter::activate() {
    if(isActive) {
        return;
    }
    isActive = true;
    this->hdc = GetDC(this->hwnd);
}

void ScreenWriter::deactivate() {
    if(!isActive) {
        std::cerr << "ScreenWriter::deactivate: drawing is inactive already";
        return;
    }
    ReleaseDC(this->hwnd, this->hdc);
    isActive = false;
}