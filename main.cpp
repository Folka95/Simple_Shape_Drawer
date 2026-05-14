// ============================================================
// main.cpp — Program entry point.
// This file creates the Windows window, sets up the menu,
// listens for user input (mouse, keyboard, menus), and hands
// every event to AppManager which does all the real work.
// ============================================================

#include <bits/stdc++.h>   // Includes every standard C++ header at once (shortcut used in competitive programming)
#include <Windows.h>       // Windows API — needed for HWND, WinMain, message loop, etc.
#include "app/app_manager.h"      // Our own class that manages all drawing state and logic
#include "rendering/menu_handler.h" // Helper that builds the top menu bar (File, Draw, Fill…)

using namespace std; // Lets us write "string" instead of "std::string", "cout" instead of "std::cout", etc.

AppManager *appManager; // Global pointer to the one AppManager object; global so WndProc can reach it from any message

// ── Called when the user clicks a menu item ──────────────────
void menuSelected(short value) {
    // value is the numeric ID Windows assigned to the clicked menu item
    // selectMainMenu(value, appManager); // old direct call — replaced by the cleaner method below
    appManager->applyMenuSelection(value); // Tell AppManager which menu item was chosen so it can react
}

// ── Called when the user right-clicks anywhere on the canvas ─
void mouseRightClick(short x, short y) {
    // x,y are pixel coordinates of the click relative to the top-left of the window
    appManager->applyRightClick(x, y); // Fill a shape or finalize a curve at this position
}

// ── Called when the user left-clicks anywhere on the canvas ──
void mouseLeftClick(short x, short y) {
    // x,y are pixel coordinates of the click
    appManager->applyLeftClick(x, y); // Place the next point of the currently selected shape
}

// ── Windows message handler (called by Windows for every event) ──
// hwnd  = handle identifying our window
// mcode = which event happened (WM_LBUTTONDOWN, WM_CLOSE, etc.)
// wp    = first extra data (depends on mcode)
// lp    = second extra data (depends on mcode)
// Returns LRESULT — Windows expects 0 for handled messages and calls DefWindowProc for everything else
LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {

        case WM_SETCURSOR: // Windows sends this every time the cursor moves over our window
            appManager->applyMouseCursor(); // Let AppManager pick which cursor shape to show (arrow, hand, crosshair…)
            return TRUE; // Return TRUE to tell Windows we handled it ourselves (prevents default cursor restore)

        case WM_COMMAND: // A menu item was clicked; Windows packs the item ID into the low 16 bits of wp
            menuSelected(LOWORD(wp)); // LOWORD extracts the lower 16 bits — that's the menu item ID
            break;

        case WM_KEYDOWN: // A keyboard key was pressed
            if (GetKeyState(VK_CONTROL) & 0x8000) { // Check if Ctrl is currently held down (bit 15 = pressed)
                if (wp == 'Z') {            // Ctrl+Z
                    appManager->undoStep(); // Undo the last drawing action
                } else if (wp == 'Y') {    // Ctrl+Y
                    appManager->redoStep(); // Redo the previously undone action
                }
            }
            break;

        case WM_RBUTTONDOWN: // Right mouse button was pressed
            // lp packs both x and y coordinates: low 16 bits = x, high 16 bits = y
            mouseRightClick(LOWORD(lp), HIWORD(lp)); // Extract x and y then call our handler
            break;

        case WM_LBUTTONDOWN: // Left mouse button was pressed
            mouseLeftClick(LOWORD(lp), HIWORD(lp)); // Extract x and y then call our handler
            break;

        case WM_CLOSE: // User clicked the X button or pressed Alt+F4
            DestroyWindow(hwnd); // Ask Windows to destroy (close) our window, which will trigger WM_DESTROY
            break;

        case WM_DESTROY: // Window has been destroyed; time to exit
            PostQuitMessage(0); // Put a WM_QUIT message in the queue so the message loop below will end
            break;

        default: // Any other Windows message we don't care about
            return DefWindowProc(hwnd, mcode, wp, lp); // Let Windows handle it the default way
    }
    return 0; // Return 0 to tell Windows we handled the message successfully
}

// ── Program entry point on Windows (replaces int main) ──────
// h   = handle to this running instance of the program
// nsh = how the window should first appear (maximized, normal, etc.)
int APIENTRY WinMain(HINSTANCE h, HINSTANCE, LPSTR, int nsh) {

    WNDCLASS wc = {}; // Zero-initialize a struct that describes our window class (type of window)

    wc.cbClsExtra = 0;                              // No extra bytes to allocate for the class — we don't need them
    wc.cbWndExtra = 0;                              // No extra bytes to allocate for each window instance
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Default cursor is the standard arrow pointer
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);         // Use the default Windows logo as our window icon
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Paint the window background solid black
    wc.hInstance = h;                               // Associate this window class with our program instance
    wc.lpfnWndProc = WndProc;                       // Tell Windows which function to call for events (our WndProc above)
    wc.lpszMenuName = NULL;                         // No menu resource from a .rc file — we build the menu in code
    wc.style = CS_HREDRAW | CS_VREDRAW;             // Redraw the whole window whenever it's resized horizontally or vertically
    wc.lpszClassName = "myclass";                   // Internal name we use to reference this window class later

    // ── Calculate a centered window that is 3/4 the size of the screen ──
    int frameWidth  = SCREEN_WIDTH  * 3 / 4; // Window width  = 75% of screen width  (SCREEN_WIDTH defined in menu_handler.h)
    int frameHeight = SCREEN_HEIGHT * 3 / 4; // Window height = 75% of screen height

    int posX = (SCREEN_WIDTH  - frameWidth)  / 2; // X offset to center the window horizontally
    int posY = (SCREEN_HEIGHT - frameHeight) / 2; // Y offset to center the window vertically

    RegisterClass(&wc); // Register the window class with Windows so CreateWindow can use it

    // ── Create the actual window ──────────────────────────────
    HWND hwnd = CreateWindow(
        wc.lpszClassName,              // Use the class we just registered
        "Simple Drawer",               // Title bar text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // Style: has title bar, system menu (X button), but NOT resizable
        posX, posY,                    // Screen position (top-left corner of the window)
        frameWidth, frameHeight,       // Size of the window
        NULL,                          // No parent window
        NULL,                          // No menu yet (we add it below)
        h,                             // Program instance handle
        0                              // No extra data passed to WM_CREATE
    );

    // ── Build the menu bar (File, Draw, Fill, Clip…) ─────────
    HMENU hMenu = CreateMenu();  // Create an empty top-level menu bar
    createAppMenu(hMenu);        // Populate it with all submenus and items (defined in menu_handler.h)

    SetMenu(hwnd, hMenu);        // Attach the finished menu to our window
    ShowWindow(hwnd, nsh);       // Make the window visible; nsh controls initial state (normal/maximized/etc.)
    UpdateWindow(hwnd);          // Force an immediate WM_PAINT so the window draws itself right away

    appManager = new AppManager(hwnd); // Create the manager that owns all drawing state, passing our window handle

    // ── Windows message loop ──────────────────────────────────
    // This loop runs continuously until the window is closed.
    // GetMessage blocks (waits) until a message arrives, then returns it in msg.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) { // Returns 0 (false) when WM_QUIT is received
        TranslateMessage(&msg);  // Convert raw keyboard input into WM_CHAR messages where applicable
        DispatchMessage(&msg);   // Send the message to WndProc for processing
    }
    return 0; // Program exits cleanly; Windows cleans up any remaining resources
}
