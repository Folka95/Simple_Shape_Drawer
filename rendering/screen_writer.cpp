#include "screen_writer.h"

#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>

ScreenWriter::ScreenWriter(HWND hwnd) {
    isActive = false;
    this->hwnd = hwnd;
    backgroundColor = RGB(0, 0, 0);
    isUserDrawn = vector< vector< bool > > (this->getWidth(), vector< bool >(this->getHeight(), false));
    screen = vector< vector< COLORREF > > (this->getWidth(), vector< COLORREF >(this->getHeight(), backgroundColor));
    cout << this->getWidth() << ' ' << this->getHeight() << endl;
}

ScreenWriter::~ScreenWriter() {
    this->deactivate();
}

void ScreenWriter::setBackgroundColor(COLORREF color) {
    backgroundColor = color;
}

void ScreenWriter::changeBackgroundColor(COLORREF color) {
    setBackgroundColor(color);
    for(int i = 0; i < this->getWidth(); i++) {
        for(int j = 0; j < this->getHeight(); j++) {
            if(!isUserDrawn[i][j]) {
                screen[i][j] = backgroundColor;
            }
        }
    }
    this->setScreen(screen, false);
}

void ScreenWriter::setPixel(int x, int y, COLORREF color) {
    if (!isActive) {
        std::cerr << "ScreenWriter::setPixel: sw is not activated" << std::endl;
        return;
    }
    if (this->outOfBounds(x, y)) {
        // std::cerr << "ScreenWriter::setPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return;
    }
    // SetPixel(this->hdc, x, y, color);
    screen[x][y] = color;
    isUserDrawn[x][y] = true;
}

COLORREF ScreenWriter::getPixel(int x, int y) {
    if (!isActive) {
        std::cerr << "ScreenWriter::getPixel: sw is not activated" << std::endl;
        return backgroundColor;
    }
    if (this->outOfBounds(x, y)) {
        // std::cerr << "ScreenWriter::getPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return backgroundColor;
    }
    return screen[x][y];
}

void ScreenWriter::clearScreen() {
    for(int y = 0; y < getHeight(); y++) {
        for(int x = 0; x < getWidth(); x++) {
            screen[x][y] = backgroundColor;
            this->isUserDrawn[x][y] = false;
        }
    }
    this->setScreen(screen, true);
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

vector< vector< COLORREF > > ScreenWriter::getScreen() {
    return screen;
}

void ScreenWriter::updateScreen() {
    this->setScreen(screen, false);
}

void ScreenWriter::setScreen(const vector<vector<COLORREF>>& screen, bool setUserFalse) {
    int width = this->getWidth();
    int height = this->getHeight();

    if (screen.size() != width || screen[0].size() != height) {
        std::cerr << "ScreenWriter::setScreenAnimated: wrong size\n";
        return;
    }

    vector<COLORREF> pixels(width * height);

    // Convert to contiguous BGRA-compatible buffer
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            COLORREF c = screen[x][y];

            BYTE r = GetRValue(c);
            BYTE g = GetGValue(c);
            BYTE b = GetBValue(c);

            // Convert RGB -> BGR for DIB
            pixels[y * width + x] =
                (r << 16) |
                (g << 8)  |
                (b);

            this->screen[x][y] = screen[x][y];
            if(setUserFalse) {
                this->isUserDrawn[x][y] = false;
            }
        }
    }

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down bitmap
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    this->activate();

    const int STEP = 20;

    for (int drawnWidth = STEP; drawnWidth <= width; drawnWidth += STEP) {

        int currentWidth = min(drawnWidth, width);

        StretchDIBits(
            this->hdc,
            0, 0,
            currentWidth, height,
            0, 0,
            currentWidth, height,
            pixels.data(),
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // Ensure final frame fully drawn
    StretchDIBits(
        this->hdc,
        0, 0,
        width, height,
        0, 0,
        width, height,
        pixels.data(),
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    this->deactivate();
}