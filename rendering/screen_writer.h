// ============================================================
// screen_writer.h — Declaration of ScreenWriter
//
// ScreenWriter is the ONLY class that talks to the Windows GDI
// (Graphics Device Interface) to put pixels on the screen.
// Every algorithm calls sw->setPixel() and sw->getPixel();
// ScreenWriter handles all the Windows-specific details
// (device contexts, bitmaps, bounds checking) so the algorithms
// themselves stay clean and platform-independent.
//
// DUAL BUFFER DESIGN:
//   - "screen" is an in-memory 2D array of COLORREF values.
//     All setPixel/getPixel calls work on this array instantly.
//   - When drawing is finished, updateScreen() / setScreen()
//     flushes the whole array to the real Windows window at once
//     using StretchDIBits (a single fast bitmap blit).
// This prevents flickering and makes animation mode possible.
// ============================================================

#pragma once          // Prevent multiple inclusions
#ifndef SCREEN_WRITER_H
#define SCREEN_WRITER_H

#include <windows.h>  // HDC, HWND, COLORREF, RECT, GetDC, ReleaseDC, SetPixel, StretchDIBits — all Windows GDI types
#include <vector>     // std::vector — used for the 2D pixel buffer and the isUserDrawn flag grid
using namespace std;

class ScreenWriter {
private:
    HDC  hdc;              // Handle to the Windows Device Context — required by GDI to draw on the window
                           // Only valid between activate() and deactivate() calls
    HWND hwnd;             // Handle to the window we are drawing into — passed in from main.cpp
    COLORREF backgroundColor; // The background color; pixels set to this are treated as "blank"
    bool isActive;         // True only between activate() and deactivate(); guards against drawing without a DC
    vector< vector< bool > > isUserDrawn; // [x][y] = true if this pixel was intentionally drawn by an algorithm
                                          // Used by changeBackgroundColor() to repaint only blank pixels
    vector< vector< COLORREF > > screen;  // [x][y] = the current color of every pixel on the canvas
                                          // This is the in-memory buffer; the real window is updated in bulk
    bool isAnimation; // If true: setPixel() draws to the Windows DC immediately (visible as it draws)
                      // If false: setPixel() only updates the buffer; window updates happen at the end

public:
    ScreenWriter(HWND hwnd);  // Constructor: stores hwnd, initializes the pixel buffer to backgroundColor
    ~ScreenWriter();           // Destructor: calls deactivate() to release any outstanding DC

    void setBackgroundColor(COLORREF color); // Update the stored background color value (does NOT repaint the screen)
    COLORREF getBackgroundColor() const;     // Return the current background color

    void changeBackgroundColor(COLORREF color); // Change the background AND repaint all blank (non-user-drawn) pixels

    void setPixel(int x, int y, COLORREF color); // Paint one pixel: writes to the buffer, and to the DC if animation is on
    COLORREF getPixel(int x, int y);              // Read one pixel's current color from the buffer

    void clearScreen(); // Repaint every pixel with backgroundColor and mark all pixels as not-user-drawn

    int  getWidth();                 // Return the current drawable width of the window (in pixels)
    int  getHeight();                // Return the current drawable height of the window (in pixels)
    bool outOfBounds(int x, int y);  // Return true if (x,y) is outside the window; used as a guard before setPixel/getPixel

    void activate();   // Acquire a Windows DC (GetDC); must be called before any GDI drawing
    void deactivate(); // Release the Windows DC (ReleaseDC); must always follow activate()

    vector< vector< COLORREF > > getScreen(); // Return a copy of the entire pixel buffer (used by softSaveScreen)
    void setScreen(const vector< vector< COLORREF > > &screen, bool setUserFalse); // Copy a pixel grid into the buffer and blit to window
                                                                                   // setUserFalse=true marks all pixels as not-user-drawn (used by clearScreen)

    void updateScreen(); // Re-blit the current buffer to the window (makes all pending changes visible)

    void setAnimation(bool isAnimated); // Toggle animation mode on or off
};

#endif //SCREEN_WRITER_H
