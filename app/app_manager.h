// ============================================================
// app_manager.h — Declaration of AppManager
//
// AppManager is the central "brain" of the whole application.
// It owns every piece of state: what shape is being drawn, which
// algorithms are selected, what colors are active, and the full
// undo/redo history. Every user action (click, menu selection,
// keyboard shortcut) goes through AppManager so it can record
// the action and respond appropriately.
// ============================================================

#pragma once          // Prevent this header being included more than once
#ifndef APP_MANAGER_H // Classic include guard — same purpose, for older compilers
#define APP_MANAGER_H

// ── Includes: everything AppManager needs to know about ───────
#include "../algorithms/clipping_algorithm.h"  // Base class for all clipping algorithms (rectangle clip, circle clip, etc.)
#include "../rendering/screen_writer.h"         // ScreenWriter manages the pixel buffer and draws to the Windows window
#include "../core/shape.h"                      // Shape base class — AppManager holds Shape* pointers polymorphically
#include "../algorithms/drawing_algorithm.h"    // Base class for all drawing algorithms (DDA, Midpoint, Polar, etc.)
#include "../algorithms/filling_algorithm.h"    // Base class for all filling algorithms (FloodFill, ScanLine, etc.)
#include "../io/action.h"                       // Action subclasses — every user interaction is recorded as an Action

#include <vector>  // std::vector — used for the action history and shape history lists
#include <stack>   // std::stack  — used for the undo and redo stacks
using namespace std;

// ── Separator macros (used in console debug output) ───────────
#define largeSeparator  string(30, '=') // "==============================" — used around major state changes
#define mediumSeparator string(20, '=') // "====================" — used around routine operations
#define smallSeparator  string(10, '=') // "==========" — used around minor messages

#include "../rendering/mouse_cursor_manager.h" // MouseCursorManager lets AppManager cycle the cursor shape

// ── AppManager class ───────────────────────────────────────────
class AppManager {
private:
    HWND hwnd;           // Handle to the application window — passed to ScreenWriter so it can draw on the correct window
    ScreenWriter *sw;    // Pointer to the pixel-buffer/renderer; AppManager owns it (created in constructor, deleted in destructor)
    MouseCursorManager mouse; // Manages the current mouse cursor shape (arrow, hand, crosshair, etc.)

    DrawingAlgorithm  *drawingAlgorithm;        // Currently selected algorithm for drawing shapes (e.g. DDA, Midpoint Circle)
    FillingAlgorithm  *fillingAlgorithm;         // Currently selected algorithm for filling shapes (e.g. FloodFill, ScanLine)
    ClippingAlgorithm *clippingAlgorithm;        // Currently selected clipping algorithm (e.g. rectangle clip, circle clip)
    DrawingAlgorithm  *clippingDrawingAlgorithm; // Separate drawing algorithm used to DRAW the clipping region boundary itself

    vector< Action* > actionHistory; // Ordered list of every action the user has taken since the last undo checkpoint
                                     // This is the "current session" history used to replay actions
    vector< Shape* >  shapeHistory;  // List of all Shape objects created so far; the last one is the active shape

    stack< vector< Action* > > undo; // Undo stack: each entry is a complete snapshot of actionHistory at a point in time
                                     // Pressing Ctrl+Z pops one snapshot and replays it
    stack< vector< Action* > > redo; // Redo stack: snapshots pushed here when the user undoes, so Ctrl+Y can replay them

    Shape *clippingRegion; // The shape that defines the clipping window; nullptr means no clipping is active
    COLORREF borderColor;  // The color used for drawing shape outlines (set via the color menu)
    COLORREF fillColor;    // The color used for filling shape interiors (set via the color menu)

    // ── Private sub-handlers ──────────────────────────────────
    // These are separated from the public methods so the public versions can handle the
    // undo/redo bookkeeping, while the private versions do just the pure action logic
    // and can be called without creating undo checkpoints (e.g. during replay).

    void applyLeftClickClippingMode(short x, short y, bool isUser);  // Handle left-click when a clipping region is active
    void applyLeftClickNoneClipping(short x, short y, bool isUser);  // Handle left-click in normal (no clipping) mode
    void reset();                                                      // Public wrapper for Private_reset

    bool animationEnabled; // Whether animation mode is on (shows pixels being drawn live vs. all at once)
    int  currentMouse;     // Index into the array of available cursor shapes; cycles on changeMouse()

    // "Private_" prefix = the real implementation; public methods call these with isUser=true,
    // undo/redo replay calls them with isUser=false to skip undo checkpoint creation
    void Private_applyLeftClick(short x, short y, bool isUser);
    void Private_applyRightClick(short x, short y, bool isUser);
    void Private_applyRightClickCurve(short x, short y, bool isUser); // Special case: right-click on a curve finalizes it
    void Private_applyMenuSelection(short choice, vector< short > data, bool isUser);
    void Private_setShape(Shape *shape, bool isUser);
    void Private_applyActions(vector< Action* > actions, bool isUser); // Replay a list of recorded actions from scratch
    void Private_reset(bool isUser); // Clear everything and return to the initial default state

    void clearUndo(); // Empty the undo stack (called when a new action makes redo invalid)
    void clearRedo(); // Empty the redo stack (called whenever the user takes a new action after undoing)
    vector< Action* > copyActionVector(vector< Action* > &actions); // Deep-copy a vector of Action* (so stacks hold independent snapshots)

public:
    AppManager(HWND _hwnd); // Constructor: stores the window handle, creates ScreenWriter, sets up defaults
    ~AppManager();          // Destructor: deletes all heap-allocated objects to prevent memory leaks

    // ── Shape and color setters ───────────────────────────────
    void setShape(Shape *shape, bool isUser = true); // Switch to drawing a new shape type (e.g. from Line to Circle)
    void setBoarderColor(COLORREF color);            // Change the border/outline color for future shapes
    void setFillColor(COLORREF color);               // Change the fill color for future right-click fills
    void setBackgroundColor(COLORREF color);         // Change the canvas background color (repaints blank pixels)

    // ── Algorithm setters ─────────────────────────────────────
    void setDrawingAlgorithm(DrawingAlgorithm *drawingAlgorithm);                                           // Replace current drawing algorithm
    void setFillingAlgorithm(FillingAlgorithm *fillingAlgorithm);                                           // Replace current filling algorithm
    void setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm, DrawingAlgorithm *clipDrawingAlgorithm, Shape *region, bool isUser); // Activate clipping mode

    // ── Algorithm removers (delete and set to nullptr) ────────
    void removeDrawingAlgorithm();   // Free and null out the drawing algorithm
    void removeFillingAlgorithm();   // Free and null out the filling algorithm
    void removeClippingAlgorithm();  // Free and null out everything related to clipping

    // ── User input handlers (called from main.cpp WndProc) ───
    void applyRightClick(short x, short y);    // User right-clicked the canvas
    void applyLeftClick(short x, short y);     // User left-clicked the canvas
    void applyMenuSelection(short choice);     // User picked a menu item (shape, algorithm, color, etc.)

    // ── Cursor ────────────────────────────────────────────────
    void changeMouse();      // Cycle to the next cursor shape in the array
    void applyMouseCursor(); // Apply the currently selected cursor (called every WM_SETCURSOR)

    void printState(); // Print the current configuration to the console (shape, algorithms, colors)

    // ── Animation ─────────────────────────────────────────────
    bool toggleAnimation();      // Flip animation on/off; returns the new state
    bool isAnimationEnabled();   // Returns whether animation is currently enabled

    // ── File operations ───────────────────────────────────────
    void clearScreen();                       // Erase all drawn content (keeps current shape/algorithm selection)
    void softSaveScreen(string filepath);     // Save the raw pixel grid to a .ssv file (like a screenshot)
    void hardSaveScreen(string filepath);     // Save the action history to a .hsv file (so it can be replayed)
    void loadScreen(string filepath);         // Load either a .ssv or .hsv file and restore the screen

    // ── Undo / Redo ───────────────────────────────────────────
    void undoStep(); // Ctrl+Z: revert to the previous undo snapshot
    void redoStep(); // Ctrl+Y: re-apply an undone action

    HWND getScreenOwner(); // Returns the window handle (used by menu handlers that need the HWND)
};

#endif // APP_MANAGER_H
