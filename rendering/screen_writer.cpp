// ============================================================
// screen_writer.cpp — Implementation of ScreenWriter
//
// ScreenWriter wraps Windows GDI so every algorithm can just
// call setPixel/getPixel without knowing anything about Windows.
// See screen_writer.h for the design overview.
// ============================================================

#include "screen_writer.h" // Our own header (ScreenWriter class declaration)

#include <iostream>  // std::cerr — for printing error messages to the console
#include <ostream>   // std::endl
#include <thread>    // std::this_thread::sleep_for — available if animation needs delays (not used currently)
#include <chrono>    // std::chrono::milliseconds — companion to thread for sleep

// ── Constructor ───────────────────────────────────────────────
ScreenWriter::ScreenWriter(HWND hwnd) {
    isActive    = false;       // DC is not held yet — must call activate() before drawing
    isAnimation = false;       // Animation mode starts disabled; shapes appear all at once
    this->hwnd  = hwnd;        // Store the window handle so we know which window to draw into
    backgroundColor = RGB(0, 0, 0); // Default background is solid black

    // Allocate the in-memory pixel buffer:
    //   isUserDrawn[x][y] = false for every pixel (no user drawing yet)
    isUserDrawn = vector< vector< bool > >(this->getWidth(), vector< bool >(this->getHeight(), false));

    //   screen[x][y] = backgroundColor for every pixel (start with a black canvas)
    screen = vector< vector< COLORREF > >(this->getWidth(), vector< COLORREF >(this->getHeight(), backgroundColor));
}

// ── Destructor ────────────────────────────────────────────────
ScreenWriter::~ScreenWriter() {
    this->deactivate(); // Release any held DC on shutdown to prevent resource leaks
}

// ── Background color management ───────────────────────────────
void ScreenWriter::setBackgroundColor(COLORREF color) {
    backgroundColor = color; // Just update the stored value — does NOT repaint anything
}

COLORREF ScreenWriter::getBackgroundColor() const {
    return backgroundColor; // Return the currently stored background color
}

// Change the background color AND repaint all blank pixels ─────
void ScreenWriter::changeBackgroundColor(COLORREF color) {
    this->activate();         // Acquire DC before writing pixels
    setBackgroundColor(color); // Store the new background color

    // Walk every pixel on the canvas
    for(int i = 0; i < this->getWidth(); i++) {
        for(int j = 0; j < this->getHeight(); j++) {
            if(!isUserDrawn[i][j]) {           // Only repaint blank (non-user-drawn) pixels
                this->setPixel(i, j, backgroundColor); // Set it to the new background color
                isUserDrawn[i][j] = false;     // Keep it marked as non-user-drawn (it's still background)
            }
        }
    }

    this->deactivate();               // Release DC after all pixels are updated
    this->setScreen(screen, false);   // Blit the updated buffer to the window
}

// ── setPixel: write one pixel ────────────────────────────────
void ScreenWriter::setPixel(int x, int y, COLORREF color) {
    if (!isActive) { // Guard: calling setPixel without a DC would be a no-op or crash
        std::cerr << "ScreenWriter::setPixel: sw is not activated" << std::endl;
        return;
    }
    if (this->outOfBounds(x, y)) { // Guard: don't try to draw outside the window boundaries
        // std::cerr << "ScreenWriter::setPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return; // Silently skip out-of-bounds pixels (algorithms naturally go out of bounds at edges)
    }
    if (isAnimation) {
        SetPixel(this->hdc, x, y, color); // In animation mode: draw immediately to the Windows DC so the pixel appears right now
    }
    screen[x][y]      = color; // Always update the in-memory buffer (so updateScreen() is always in sync)
    isUserDrawn[x][y] = true;  // Mark this pixel as user-drawn so changeBackgroundColor() won't erase it
}

// ── getPixel: read one pixel ──────────────────────────────────
COLORREF ScreenWriter::getPixel(int x, int y) {
    if (!isActive) { // Guard: calling without a DC
        std::cerr << "ScreenWriter::getPixel: sw is not activated" << std::endl;
        return backgroundColor; // Return background as a safe fallback color
    }
    if (this->outOfBounds(x, y)) { // Guard: reading outside the window
        // std::cerr << "ScreenWriter::getPixel: coordinates (" << x << ", " << y << ") are out of bounds" << std::endl;
        return backgroundColor; // Return background as a safe fallback (flood fill uses this to stop at edges)
    }
    return screen[x][y]; // Return the color from the in-memory buffer (fast — no GDI call needed)
}

// ── clearScreen: reset the canvas ────────────────────────────
void ScreenWriter::clearScreen() {
    this->activate(); // Acquire DC

    // Repaint every pixel with the background color
    for(int y = 0; y < getHeight(); y++) {
        for(int x = 0; x < getWidth(); x++) {
            setPixel(x, y, backgroundColor); // Set to background (this also updates the buffer)
            this->isUserDrawn[x][y] = false; // Mark pixel as blank/not-user-drawn
        }
    }

    this->deactivate();             // Release DC
    this->setScreen(screen, true);  // Blit the cleared buffer to the window; true = mark all pixels as not-user-drawn
}

// ── Window size helpers ───────────────────────────────────────
int ScreenWriter::getWidth() {
    RECT rect;
    GetClientRect(this->hwnd, &rect); // Ask Windows for the drawable area of our window (excludes title bar/borders)
    return rect.right - rect.left;    // Width = right edge x minus left edge x
}

int ScreenWriter::getHeight() {
    RECT rect;
    GetClientRect(this->hwnd, &rect); // Same as above but for height
    return rect.bottom - rect.top;    // Height = bottom edge y minus top edge y
}

// ── Bounds check ──────────────────────────────────────────────
bool ScreenWriter::outOfBounds(int x, int y) {
    // Returns true if the coordinate is outside the drawable area
    // Used as a guard in setPixel, getPixel, and flood fill algorithms
    return (x < 0 || y < 0 || x >= getWidth() || y >= getHeight());
}

// ── DC lifecycle ──────────────────────────────────────────────
void ScreenWriter::activate() {
    if(isActive) {
        return; // Already have a DC — don't acquire a second one (would leak)
    }
    isActive  = true;
    this->hdc = GetDC(this->hwnd); // Ask Windows for a Device Context tied to our window
                                   // This is the "pen and paper" we draw on via GDI calls
}

void ScreenWriter::deactivate() {
    if(!isActive) {
        std::cerr << "ScreenWriter::deactivate: drawing is inactive already";
        return; // Guard: can't release a DC we don't hold
    }
    ReleaseDC(this->hwnd, this->hdc); // Return the DC to Windows — must always pair with GetDC()
    isActive = false;                  // Mark as inactive so activate() will acquire a fresh one next time
}

// ── Buffer access ──────────────────────────────────────────────
vector< vector< COLORREF > > ScreenWriter::getScreen() {
    return screen; // Return a full copy of the pixel buffer (used by softSaveScreen to write to file)
}

void ScreenWriter::updateScreen() {
    this->setScreen(screen, false); // Re-blit the current buffer to the window without resetting isUserDrawn
                                    // Called after every drawing operation to make changes visible
}

void ScreenWriter::setAnimation(bool isAnimated) {
    this->isAnimation = isAnimated; // Toggle animation mode; affects whether setPixel() draws live or deferred
}

// ── setScreen: blit a pixel buffer to the window ─────────────
// This is the most complex method — it converts the buffer to a
// Windows DIB (Device Independent Bitmap) and blasts it to the
// screen in one call, which is much faster than SetPixel() in a loop.
void ScreenWriter::setScreen(const vector<vector<COLORREF>>& screen, bool setUserFalse) {
    int width  = this->getWidth();
    int height = this->getHeight();

    // Validate the incoming buffer dimensions match the window size
    if (screen.size() != width || screen[0].size() != height) {
        std::cerr << "ScreenWriter::setScreenAnimated: wrong size\n";
        return;
    }

    // Allocate a flat (1D) pixel array: Windows DIB expects pixels in row-major order
    vector<COLORREF> pixels(width * height);

    // Convert our [x][y] 2D array (column-major) into a row-major array expected by StretchDIBits
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            COLORREF c = screen[x][y]; // Read pixel color from our column-major buffer

            BYTE r = GetRValue(c); // Extract the Red   component (bits 0–7 of COLORREF)
            BYTE g = GetGValue(c); // Extract the Green component (bits 8–15 of COLORREF)
            BYTE b = GetBValue(c); // Extract the Blue  component (bits 16–23 of COLORREF)

            // Windows DIBs store pixels as BGR (not RGB). StretchDIBits reads 0xBBGGRR00.
            // We reassemble the bytes in BGR order:
            pixels[y * width + x] =
                (r << 16) |  // Red goes into bits 16–23 (DIB "blue" position — Windows reverses it internally)
                (g << 8)  |  // Green goes into bits 8–15
                (b);         // Blue goes into bits 0–7

            this->screen[x][y] = screen[x][y]; // Update our internal buffer to stay in sync

            if(setUserFalse) {
                this->isUserDrawn[x][y] = false; // If requested (e.g. clearScreen), mark pixel as not-user-drawn
            }
        }
    }

    // Describe the bitmap format to Windows
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER); // Size of this header struct — required by Windows
    bmi.bmiHeader.biWidth       = width;                    // Bitmap width in pixels
    bmi.bmiHeader.biHeight      = -height;                  // NEGATIVE height = top-down bitmap (row 0 is at the top)
                                                            // Positive height would mean row 0 is at the bottom (upside-down)
    bmi.bmiHeader.biPlanes      = 1;                        // Always 1 for screen bitmaps
    bmi.bmiHeader.biBitCount    = 32;                       // 32 bits per pixel (BGRA format, A is unused here)
    bmi.bmiHeader.biCompression = BI_RGB;                   // No compression — raw pixel data

    this->activate(); // Acquire Windows DC before calling StretchDIBits

    // Blit the entire pixel array to the window in one fast call
    StretchDIBits(
        this->hdc,         // Target: the window's device context
        0, 0,              // Destination top-left corner (0,0 = top-left of client area)
        width, height,     // Destination rectangle size (same as source — no scaling)
        0, 0,              // Source top-left corner in the pixel array
        width, height,     // Source rectangle size
        pixels.data(),     // Pointer to the flat pixel array we built above
        &bmi,              // Pointer to the bitmap format descriptor
        DIB_RGB_COLORS,    // The pixel data uses literal RGB values (not palette indices)
        SRCCOPY            // Raster operation: simply copy source to destination
    );

    this->deactivate(); // Release the Windows DC
}
