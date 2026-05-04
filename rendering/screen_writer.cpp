#include "screen_writer.h"

#include <iostream>
#include <ostream>

ScreenWriter::ScreenWriter(HWND hwnd) {
    this->hdc = nullptr;
    this->hwnd = hwnd;
    backgroundColor = RGB(0, 0, 0);
}

ScreenWriter::~ScreenWriter() {
    this->deactivate();
}

void ScreenWriter::setBackgroundColor(COLORREF color) {
    backgroundColor = color;
}

void ScreenWriter::setPixel(int x, int y, COLORREF color) {
    if(!this->hdc) {
        std::cerr << "ScreenWriter::setPixel: sw is not acticated";
        return;
    }
    SetPixel(*(this->hdc), x, y, color);
}

void ScreenWriter::clearScreen() {
    HDC hdc = GetDC(hwnd);

    RECT rect;
    GetClientRect(this->hwnd, &rect);

    HBRUSH brush = CreateSolidBrush(backgroundColor);
    FillRect(hdc, &rect, brush);

    DeleteObject(brush);
    ReleaseDC(this->hwnd, hdc);
}

int ScreenWriter::getWidth() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    return rect.right - rect.left;
}

int ScreenWriter::getHeight() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    return rect.bottom - rect.top;
}

bool ScreenWriter::outOfBounds(int x, int y) {
    return (x < 0 || y < 0 || x >= getWidth() || y >= getHeight());
}

void ScreenWriter::activate() {
    if(this->hdc) {
        return;
    }
    *(this->hdc) = GetDC(this->hwnd);
}

void ScreenWriter::deactivate() {
    if(!this->hdc) {
        std::cerr << "ScreenWriter::deactivate: hdc is null";
        return;
    }
    ReleaseDC(this->hwnd, *(this->hdc));
    this->hdc = nullptr;
}