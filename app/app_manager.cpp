// ============================================================
// app_manager.cpp — Implementation of AppManager
//
// AppManager is the orchestrator. It sits between the raw Windows
// events (mouse clicks, menu picks, keyboard shortcuts) and the
// actual drawing/filling/clipping algorithms. Its main jobs are:
//
//   1. DISPATCH: route each user action to the right algorithm
//   2. RECORD:   push every action into actionHistory so undo/redo work
//   3. STATE:    keep track of the current shape, algorithms, and colors
// ============================================================

#include "app_manager.h"    // Our own header
#include <bits/stdc++.h>    // All standard headers (filesystem, string, sort, etc.)

#include "../algorithms/drawing/polygon/polygon_drawing_algorithm.h" // Default drawing algorithm (polygon)
#include "../io/action.h"          // LeftClickAction, RightClickAction, MenuSelectAction classes
#include "../io/file_manager.h"    // FileManager::saveActions, loadActions, saveScreen, loadScreen
#include "../rendering/menu_handler.h" // selectMainMenu(), mainMenuDecoder(), subMenuDecoder() — parse menu choices

using namespace std;

// ── Constructor ───────────────────────────────────────────────
AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;                  // Store the window handle so we can pass it to ScreenWriter
    this->animationEnabled = false;      // Animation starts disabled — shapes appear instantly
    this->sw = new ScreenWriter(_hwnd);  // Create the pixel buffer/renderer for this window
    sw->setAnimation(this->animationEnabled); // Sync animation state into ScreenWriter

    // Initialize all algorithm pointers to null — user picks algorithms via the menu
    this->drawingAlgorithm        = nullptr;
    this->fillingAlgorithm        = nullptr;
    this->clippingAlgorithm       = nullptr;
    this->clippingRegion          = nullptr;
    this->clippingDrawingAlgorithm= nullptr;

    borderColor = RGB(255, 255, 255); // Default border color: white
    fillColor   = RGB(255, 255, 255); // Default fill color: white

    shapeHistory.push_back(new PolygonShape<1>()); // Start with a 1-vertex polygon as the default shape type
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm()); // Default algorithm: polygon drawing

    this->undo.push(this->copyActionVector(this->actionHistory)); // Push the initial empty state as the first undo checkpoint
    currentMouse = 0; // Start with the first cursor (arrow)
}

// ── Destructor ────────────────────────────────────────────────
AppManager::~AppManager() {
    delete this->sw;                      // Free the ScreenWriter (releases pixel buffer)
    delete this->drawingAlgorithm;        // Free whichever drawing algorithm was selected
    delete this->fillingAlgorithm;        // Free whichever filling algorithm was selected
    delete this->clippingAlgorithm;       // Free whichever clipping algorithm was selected
    delete this->clippingRegion;          // Free the clipping region shape

    shapeHistory.clear();                 // Clear the shape history list
    actionHistory.clear();                // Clear the action history list

    shapeHistory.push_back(new PolygonShape<1>()); // Push a fresh default shape (prevents dangling state)
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm()); // Restore a default algorithm to keep invariants valid

    this->clearRedo(); // Empty the redo stack, freeing all its Action copies
    this->clearUndo(); // Empty the undo stack, freeing all its Action copies
}

// ── Reset: clear everything and return to default state ───────
void AppManager::Private_reset(bool isUser) {
    this->removeClippingAlgorithm(); // Delete clipping algo + region (sets all three clipping pointers to null)
    this->removeDrawingAlgorithm();  // Delete current drawing algorithm
    this->removeFillingAlgorithm();  // Delete current filling algorithm

    sw->setBackgroundColor(RGB(0, 0, 0)); // Reset background to black
    sw->clearScreen();                     // Repaint the whole canvas with the background color

    shapeHistory.clear();   // Delete all shape history entries
    actionHistory.clear();  // Clear the recorded actions

    shapeHistory.push_back(new PolygonShape<1>()); // Restore the default 1-vertex polygon as the active shape
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm()); // Restore default drawing algorithm

    if(isUser) { // Only update undo/redo stacks when reset is triggered by the user (not during undo replay)
        this->clearRedo();
        this->clearUndo();
        this->undo.push(this->copyActionVector(this->actionHistory)); // Push a fresh empty checkpoint
    }
}

void AppManager::reset() {
    Private_reset(true); // Public wrapper — always treats this as a user action
}

// ── Shape management ──────────────────────────────────────────
void AppManager::Private_setShape(Shape *shape, bool isUser) {
    // If the current shape doesn't have enough points to draw yet, discard it before switching
    if(!shapeHistory.empty() && !shapeHistory.back()->isEnoughToDraw()) {
        shapeHistory.pop_back(); // Discard the incomplete shape (e.g. a line with only one point)
    }
    shapeHistory.push_back(shape); // Add the new shape type to the history

    if(isUser) { // Only print the description for real user actions, not undo replay
        cout << mediumSeparator << endl;
        cout << shape->getDescription() << endl; // Print the usage hint (e.g. "Click center then edge")
        cout << mediumSeparator << endl;
    }
}

void AppManager::setShape(Shape *shape, bool isUser) {
    Private_setShape(shape, isUser); // Public wrapper delegates to the private implementation
}

// ── Color setters ──────────────────────────────────────────────
void AppManager::setBoarderColor(COLORREF color) {
    borderColor = color; // Store new border color; applied to the next shape drawn
}

void AppManager::setFillColor(COLORREF color) {
    fillColor = color; // Store new fill color; applied on the next right-click fill
}

void AppManager::setBackgroundColor(COLORREF color) {
    sw->changeBackgroundColor(color); // Repaint all blank pixels with the new background color
}

// ── Algorithm setters ──────────────────────────────────────────
void AppManager::setFillingAlgorithm(FillingAlgorithm *fillingAlgorithm) {
    this->removeFillingAlgorithm();    // Free the old one first to avoid a memory leak
    this->fillingAlgorithm = fillingAlgorithm; // Store the new one
}

void AppManager::setDrawingAlgorithm(DrawingAlgorithm *drawingAlgorithm) {
    this->removeDrawingAlgorithm();    // Free the old one first
    this->drawingAlgorithm = drawingAlgorithm; // Store the new one
}

void AppManager::setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm, DrawingAlgorithm *clipDrawingAlgorithm, Shape *region, bool isUser) {
    this->removeClippingAlgorithm(); // Free old clipping state (algorithm + region + drawing algo)
    this->clippingAlgorithm        = clippingAlgorithm;       // The algorithm that clips shapes to the region
    this->clippingDrawingAlgorithm = clipDrawingAlgorithm;    // The algorithm that draws the clipping region boundary
    this->clippingRegion           = region;                  // The shape defining what region is allowed

    if(isUser) { // Print debug info for real user actions
        cout << largeSeparator << endl;
        cout << "# This is the clipping region ---- " << endl;
        cout << this->clippingRegion->getDescription() << endl;
        cout << largeSeparator << endl;
    }
}

// ── Algorithm removers ─────────────────────────────────────────
void AppManager::removeDrawingAlgorithm() {
    if(this->drawingAlgorithm != nullptr) {
        delete this->drawingAlgorithm; // Free heap memory (algorithms are allocated with new)
    }
    this->drawingAlgorithm = nullptr; // Null out the pointer so we know no algorithm is active
}

void AppManager::removeFillingAlgorithm() {
    if(this->fillingAlgorithm != nullptr) {
        delete this->fillingAlgorithm;
    }
    this->fillingAlgorithm = nullptr;
}

void AppManager::removeClippingAlgorithm() {
    if(this->clippingAlgorithm != nullptr) {
        delete this->clippingAlgorithm; // Free the clipping algorithm object
    }
    if(this->clippingRegion != nullptr) {
        delete this->clippingRegion;    // Free the region shape object
    }
    if(this->clippingDrawingAlgorithm != nullptr) {
        delete this->clippingDrawingAlgorithm; // Free the algorithm used to draw the region boundary
    }
    // Null all three to signal "no clipping active"
    this->clippingAlgorithm        = nullptr;
    this->clippingRegion           = nullptr;
    this->clippingDrawingAlgorithm = nullptr;
}

// ── Right-click: finalize a curve ─────────────────────────────
// Called when the user right-clicks while a curve is the active shape.
// Right-click = "I'm done adding control points; draw the curve now."
void AppManager::Private_applyRightClickCurve(short x, short y, bool isUser) {
    // Guard checks: all required state must be present
    if(clippingRegion != nullptr) {
        cerr << "AppManager::Private_applyRightClickCurve: clippingRegion is NOT null" << endl;
        return; // Curves can't be finalized while a clipping region is active
    }
    if(clippingAlgorithm != nullptr) {
        cerr << "AppManager::Private_applyRightClickCurve: clippingAlgorithm is NOT null" << endl;
        return;
    }
    if(shapeHistory.empty()) {
        cerr << "AppManager::Private_applyRightClickCurve: shapeHistory is empty" << endl;
        return;
    }
    if(drawingAlgorithm == nullptr) {
        cerr << "AppManager::Private_applyRightClickCurve: drawingAlgorithm is null" << endl;
        return;
    }

    this->shapeHistory.back()->takeAction(1); // Signal the curve shape to close/finalize itself (action ID 1 = finalize)

    if(!shapeHistory.back()->isEnoughToDraw()) {
        cerr << "AppManager::Private_applyRightClickCurve: not enough to draw" << endl;
        return; // Not enough control points yet to produce a curve
    }

    shapeHistory.back()->borderColor = borderColor;          // Apply the current border color to the curve
    drawingAlgorithm->draw(*shapeHistory.back(), sw);         // Draw the finalized curve using the selected algorithm

    actionHistory.push_back(new RightClickAction(3, x, y)); // Record this action (rank 3 = visible drawing occurred)

    if(isUser) { // Real user action: create an undo checkpoint and refresh the screen
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo(); // Invalidate redo history since user took a new action
        this->sw->updateScreen(); // Flush the buffer to the window
    }
}

// ── Right-click: fill a shape ─────────────────────────────────
// Called when the user right-clicks inside any non-curve shape to fill it.
void AppManager::Private_applyRightClick(short x, short y, bool isUser) {
    if(shapeHistory.empty()) {
        cerr << "AppManager::Private_applyRightClick: shapeHistory is empty" << endl;
        return;
    }

    // Special case: if active shape is a curve, delegate to the curve handler
    if(shapeHistory.back()->getType() == SHAPE_CURVE) {
        Private_applyRightClickCurve(x, y, isUser);
        return;
    }

    if(fillingAlgorithm == nullptr) {
        cerr << "AppManager::Private_applyRightClick: fillingAlgorithm is null" << endl;
        return; // Can't fill without an algorithm selected
    }

    int actionRank = 1; // Default rank = 1 (no fill happened); will be upgraded to 3 if we fill

    // Sort shapes by area (smallest first) so that clicking inside overlapping shapes
    // fills the SMALLEST one that contains the click — most intuitive behavior
    vector< Shape* > tmpShapes = shapeHistory; // Copy so we don't reorder the real history
    stable_sort(tmpShapes.begin(), tmpShapes.end(), [&](const Shape *a, const Shape *b) {
       return a->getArea() < b->getArea(); // Lambda: sort ascending by area
    });

    for(Shape* shape : tmpShapes) {
        if(shape->isEnoughToDraw() && shape->isInside(Point(x, y))) { // Find smallest shape containing the click
            shape->fillColor = fillColor;                              // Apply the current fill color to this shape
            fillingAlgorithm->fill(*shape, *clippingRegion, Point(x, y), sw); // Run the fill algorithm
            actionRank = 3; // Upgrade rank: a visible fill happened
            break;          // Stop after filling the first (smallest) matching shape
        }
    }

    actionHistory.push_back(new RightClickAction(actionRank, x, y)); // Record the action

    if(isUser) {
        if(actionRank == 3) { // Only create an undo checkpoint if something actually changed on screen
            this->undo.push(this->copyActionVector(this->actionHistory));
            this->clearRedo();
        }
        this->sw->updateScreen(); // Make the fill visible
    }
}

void AppManager::applyRightClick(short x, short y) {
    Private_applyRightClick(x, y, true); // Public wrapper — always a real user action
}

// ── Left-click in clipping mode ───────────────────────────────
// While a clipping region is active, left clicks do two things:
//   1. If the clipping region boundary isn't drawn yet, add points to it
//   2. Once the region is drawn, left-click places new shapes and immediately clips them
void AppManager::applyLeftClickClippingMode(short x, short y, bool isUser) {
    // All required pointers must be non-null to proceed
    if(clippingRegion == nullptr) {
        cerr << "AppManager::applyLeftClickClippingMode: clippingRegion is null" << endl;
        return;
    }
    if(clippingAlgorithm == nullptr) {
        cerr << "AppManager::applyLeftClickClippingMode: clippingAlgorithm is null" << endl;
        return;
    }
    if(this->clippingDrawingAlgorithm == nullptr) {
        cerr << "AppManager::applyLeftClickClippingMode: clippingDrawingAlgorithm is null" << endl;
        return;
    }
    if(shapeHistory.empty()) {
        cerr << "AppManager::applyLeftClickClippingMode: history is empty" << endl;
        return;
    }

    int actionRank = 1; // Default: no visible change

    if(this->clippingRegion->isEnoughToDraw()) {
        // The clipping region is fully defined — apply clipping to the shape being drawn
        if(shapeHistory.back()->isEnoughToDraw()) {
            // The last shape is complete; clone it to start drawing another instance of the same type
            shapeHistory.push_back(shapeHistory.back()->clone());
            shapeHistory.back()->clear(); // Clear the clone's points — it's a fresh shape of the same type
        }
        if(shapeHistory.back()->addPoint(Point(x, y))) { // Add the clicked point; returns true when shape is complete
            shapeHistory.back()->borderColor = borderColor;
            clippingAlgorithm->clip(*shapeHistory.back(), *this->clippingRegion, sw); // Clip and draw
            actionRank = 3; // A shape was drawn
        }
    }
    else {
        // The clipping region isn't complete yet — add points to it first
        if(this->clippingRegion->addPoint(Point(x, y))) { // Returns true when the region shape is complete
            this->clippingRegion->borderColor = borderColor;
            this->clippingDrawingAlgorithm->draw(*this->clippingRegion, sw); // Draw the clipping boundary
            actionRank = 3; // The clipping region was drawn
        }
    }

    actionHistory.push_back(new LeftClickAction(actionRank, x, y)); // Record action

    if(isUser && actionRank == 3) { // Only checkpoint if something actually changed
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo();
    }
}

// ── Left-click in normal (no clipping) mode ───────────────────
// Each left-click adds one point to the current shape.
// When the shape has enough points, it gets drawn immediately.
void AppManager::applyLeftClickNoneClipping(short x, short y, bool isUser) {
    // Guards: can't proceed without a shape and drawing algorithm
    if(clippingRegion != nullptr) {
        cerr << "AppManager::applyLeftClickNoneClipping: clippingRegion is NOT null" << endl;
        return;
    }
    if(clippingAlgorithm != nullptr) {
        cerr << "AppManager::applyLeftClickNoneClipping: clippingAlgorithm is NOT null" << endl;
        return;
    }
    if(shapeHistory.empty()) {
        cerr << "AppManager::applyLeftClick: shapeHistory is empty" << endl;
        return;
    }
    if(drawingAlgorithm == nullptr) {
        cerr << "AppManager::applyLeftClickNoneClipping: drawingAlgorithm is null" << endl;
        return;
    }

    // If the last shape is already complete, clone it to start fresh
    if(shapeHistory.back()->isEnoughToDraw()) {
        shapeHistory.push_back(shapeHistory.back()->clone()); // Clone preserves the shape TYPE
        shapeHistory.back()->clear(); // Clear points so the new instance starts empty
    }

    int actionRank = 1; // Default: no draw happened yet

    if(shapeHistory.back()->addPoint(Point(x, y))) { // Add the clicked point; returns true when the shape is complete
        shapeHistory.back()->borderColor = borderColor; // Apply the current border color
        drawingAlgorithm->draw(*shapeHistory.back(), sw); // Draw the completed shape using the active algorithm
        actionRank = 3; // A shape was drawn — upgrade the rank
    }

    actionHistory.push_back(new LeftClickAction(actionRank, x, y)); // Record this action

    if(isUser && actionRank == 3) { // Create undo checkpoint only if something was drawn
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo();
    }
}

// ── Route left-click to the correct handler ───────────────────
void AppManager::Private_applyLeftClick(short x, short y, bool isUser) {
    if(clippingAlgorithm == nullptr) {
        applyLeftClickNoneClipping(x, y, isUser); // No clipping active → normal mode
    }
    else {
        applyLeftClickClippingMode(x, y, isUser); // Clipping active → clipping mode
    }
    if(isUser) {
        this->sw->updateScreen(); // Flush buffer to window after every real user click
    }
}

void AppManager::applyLeftClick(short x, short y) {
    Private_applyLeftClick(x, y, true); // Public wrapper
}

// ── Menu selection handler ────────────────────────────────────
void AppManager::Private_applyMenuSelection(short choice, vector< short > data, bool isUser) {
    // Delegate to the menu handler which parses the choice and calls the appropriate setters on AppManager
    vector< short > result = selectMainMenu(choice, this, data, isUser);

    // Some menu items handle their own undo/redo and don't need to be recorded here — skip them:
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_LOAD)       return; // Load replays actions itself
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_HARD_SAVE)  return; // Save doesn't change state
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_SOFT_SAVE)  return; // Save doesn't change state
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_UNDO)       return; // Undo handled by undoStep()
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_REDO)       return; // Redo handled by redoStep()
    if(mainMenuDecoder(choice) == PREFERENCES_MENU && subMenuDecoder(choice) == PREFERENCES_TOGGLE_ANIMATION) return; // Not undoable
    if(mainMenuDecoder(choice) == PREFERENCES_MENU && subMenuDecoder(choice) == PREFERENCES_MOUSE_SHAPE)      return; // Not undoable

    // Build the action payload: [menuChoice, ...extra result data]
    vector< short > tmp = result;
    tmp.insert(tmp.begin(), choice); // Prepend the menu ID so replay knows which menu to call
    actionHistory.push_back(new MenuSelectAction(2, tmp)); // Rank 2 = setting change (not raw drawing)

    if(isUser) {
        if(mainMenuDecoder(choice) == FILE_MENU || mainMenuDecoder(choice) == PREFERENCES_MENU) {
            undo.push(this->copyActionVector(this->actionHistory)); // Checkpoint for file/pref changes
            this->clearRedo();
        }
        this->sw->updateScreen(); // Make any visual changes visible
        this->printState();       // Print current state to console for debugging
    }
}

void AppManager::applyMenuSelection(short choice) {
    Private_applyMenuSelection(choice, {}, true); // Public wrapper — no extra data, real user action
}

// ── Console state dump ────────────────────────────────────────
// Prints a summary of all current settings — useful during a demo
void AppManager::printState() {
    cout << "==============================" << endl;
    cout << "# Shapes :" << endl;
    cout << "  Shape : " << (shapeHistory.empty() ? "None" : shapeHistory.back()->getName()) << endl;
    cout << "  Clipping Region Shape: " << (clippingRegion ? clippingRegion->getName() : "None") << endl;
    cout << "# Algorithms :" << endl;
    cout << "  Drawing Algorithm : "  << (drawingAlgorithm  ? drawingAlgorithm->getName()  : "None") << endl;
    cout << "  Filling Algorithm : "  << (fillingAlgorithm  ? fillingAlgorithm->getName()  : "None") << endl;
    cout << "  Clipping algorithm : " << (clippingAlgorithm ? clippingAlgorithm->getName() : "None") << endl;
    cout << "# Colors :" << endl;
    // GetRValue/GetGValue/GetBValue extract the R, G, B components from a COLORREF for display
    cout << "  Background Color : RGB(" << (int)GetRValue(sw->getBackgroundColor()) << ", " << (int)GetGValue(sw->getBackgroundColor()) << ", " << (int)GetBValue(sw->getBackgroundColor()) << ")" << endl;
    cout << "  Boarder Color : RGB("    << (int)GetRValue(borderColor) << ", " << (int)GetGValue(borderColor) << ", " << (int)GetBValue(borderColor) << ")" << endl;
    cout << "  Filling Color : RGB("    << (int)GetRValue(fillColor)   << ", " << (int)GetGValue(fillColor)   << ", " << (int)GetBValue(fillColor)   << ")" << endl;
    cout << "==============================" << endl;
}

// ── Clear screen ──────────────────────────────────────────────
void AppManager::clearScreen() {
    cout << mediumSeparator << endl;
    cout << "Clearing screen" << endl;
    cout << mediumSeparator << endl;

    sw->clearScreen(); // Repaint all pixels with the background color

    Shape *current = shapeHistory.back()->clone(); // Clone the current shape TYPE to keep it active after clear
    this->shapeHistory.clear();                     // Remove all old shape entries
    current->clear();                               // Clear the cloned shape's points — it starts fresh
    shapeHistory.push_back(current);                // Add the fresh shape back as the new active shape
}

// ── Hard save: save the action history ───────────────────────
// A "hard save" stores the list of actions so the drawing can be
// replayed exactly. File extension must be .hsv.
void AppManager::hardSaveScreen(string filepath) {
    if(filepath.empty()) return; // No file path — nothing to do

    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".hsv") {
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Hard saving screen to (" << p.filename().string() << ")" << endl; // Show just the filename
        cout << mediumSeparator << endl;

        // Clone the action history so we can trim it without affecting the live history
        vector< Action* > tmp;
        for(Action *action : actionHistory) {
            tmp.push_back(action->clone()); // Deep copy each action
        }
        // If the last action has rank < 2 (e.g. an incomplete click), remove it — it can't be replayed usefully
        if(!tmp.empty() && tmp.back()->getRank() < 2) {
            tmp.pop_back();
        }
        FileManager::saveActions(tmp, filepath); // Write the action list to the .hsv file
    }
    else {
        cerr << "AppManager::hardSaveScreen: filepath is invalid" << endl;
    }
}

// ── Soft save: save the pixel grid ───────────────────────────
// A "soft save" takes a screenshot of the current pixels.
// File extension must be .ssv.
void AppManager::softSaveScreen(string filepath) {
    if(filepath.empty()) return;

    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".ssv") {
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Soft saving screen to (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;

        vector< vector< COLORREF > > screen = sw->getScreen(); // Get a copy of the full pixel buffer
        FileManager::saveScreen(screen, filepath);              // Write it to the .ssv file
    }
    else {
        cerr << "AppManager::softSaveScreen: filepath is invalid" << endl;
    }
}

// ── Replay a list of actions from scratch ────────────────────
// Used by undo, redo, and hard-load to reconstruct the drawing state.
void AppManager::Private_applyActions(vector< Action* > actions, bool isUser) {
    actionHistory.clear(); // Wipe current history — we're about to rebuild it
    shapeHistory.clear();  // Wipe current shapes — they'll be recreated by replay

    if(!isUser) {
        sw->setAnimation(false); // Disable animation during replay so it doesn't flicker
    }

    this->Private_reset(isUser); // Full reset to default state before replay

    for(Action *action : actions) {
        if(!isUser) {
            sw->setAnimation(false); // Keep animation off for each replayed action
        }

        // Dispatch each action to the correct handler based on its type
        if(action->getActionType() == ACTION_LEFT_CLICK) {
            this->Private_applyLeftClick(action->getData()[0], action->getData()[1], false); // Replay left click at stored coordinates
        }
        else if(action->getActionType() == ACTION_RIGHT_CLICK) {
            this->Private_applyRightClick(action->getData()[0], action->getData()[1], false); // Replay right click
        }
        else if(action->getActionType() == ACTION_MENU_SELECT) {
            // data[0] = menu ID, data[1..] = extra parameters returned by the menu handler
            vector< short > tmp;
            for(int i = 1; i < action->getData().size(); i++) {
                tmp.push_back(action->getData()[i]); // Collect extra parameters (skip data[0] which is the menu ID)
            }
            this->Private_applyMenuSelection(action->getData()[0], tmp, false); // Replay menu selection
        }
        else {
            cerr << "AppManager::Private_applyActions: unknown action type" << endl;
        }
    }

    this->sw->updateScreen(); // After full replay, flush the final frame to the window
}

// ── Load a saved file ─────────────────────────────────────────
void AppManager::loadScreen(string filepath) {
    if(filepath.empty()) return;

    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".ssv") {
        // Soft save: directly load pixel data — no replay, just set the buffer
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Loading soft save file (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;

        actionHistory.clear();
        shapeHistory.clear();
        this->reset(); // Reset state before loading a pixel dump

        vector< vector< COLORREF > > screen = FileManager::loadScreen(filepath); // Read pixel grid from file
        sw->setScreen(screen, true); // Set the pixel buffer from the loaded data; true = mark all as not-user-drawn
    }
    else if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".hsv") {
        // Hard save: load and replay the recorded actions
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Loading hard save file (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;

        Private_applyActions(FileManager::loadActions(filepath), true); // Replay all saved actions
    }
    else {
        cerr << "AppManager::loadScreen: invalid file extension" << endl;
        return;
    }
}

// ── Undo / Redo stack helpers ─────────────────────────────────
void AppManager::clearUndo() {
    while(!undo.empty()) {
        undo.pop(); // Pop every snapshot off the undo stack (frees memory)
    }
}

void AppManager::clearRedo() {
    while(!redo.empty()) {
        redo.pop(); // Pop every snapshot off the redo stack
    }
}

// Deep-copy a vector of Action*: needed so each undo/redo snapshot is independent
vector< Action* > AppManager::copyActionVector(vector< Action* > &actions) {
    vector< Action* > tmp;
    for(Action *action : actions) {
        tmp.push_back(action->clone()); // Clone each action so changes to the original don't affect the snapshot
    }
    return tmp;
}

// ── Undo ─────────────────────────────────────────────────────
void AppManager::undoStep() {
    if(undo.size() > 1) { // Must have at least 2 entries: the initial state + at least one checkpoint to revert
        redo.push(this->copyActionVector(undo.top())); // Save current state onto redo before removing it
        undo.pop();                                     // Remove the current snapshot (revert to the one below)
        cout << mediumSeparator << endl;
        cout << "Undoing last operation..." << endl;
        cout << mediumSeparator << endl;
    }
    Private_applyActions(undo.top(), false); // Replay all actions up to the now-top undo snapshot
    sw->setAnimation(isAnimationEnabled()); // Restore animation setting (replay forced it off)
}

// ── Redo ─────────────────────────────────────────────────────
void AppManager::redoStep() {
    if(!redo.empty()) { // Only redo if there is something to re-apply
        undo.push(this->copyActionVector(redo.top())); // Move the redo snapshot back onto the undo stack
        redo.pop();
        cout << mediumSeparator << endl;
        cout << "Redoing last operation..." << endl;
        cout << mediumSeparator << endl;
    }
    Private_applyActions(undo.top(), false); // Replay up to the now-top undo snapshot (which includes the re-done action)
    sw->setAnimation(isAnimationEnabled()); // Restore animation
}

// ── Mouse cursor cycling ──────────────────────────────────────
void AppManager::changeMouse() {
    static const LPCTSTR cursors[] = {
        IDC_ARROW,   // Standard pointer arrow
        IDC_HAND,    // Hand (pointing finger)
        IDC_CROSS,   // Crosshair (precision drawing)
        IDC_SIZEALL  // Four-direction move arrow
    };
    currentMouse = (currentMouse + 1) % 4; // Advance to the next cursor; wrap around after the last one
    mouse.setSystemCursor(cursors[currentMouse]); // Load and apply the selected system cursor
}

void AppManager::applyMouseCursor() {
    mouse.apply(); // Re-apply the currently stored cursor (called every WM_SETCURSOR to prevent Windows reverting it)
}

// ── Animation toggle ──────────────────────────────────────────
bool AppManager::toggleAnimation() {
    animationEnabled = !animationEnabled;           // Flip the boolean
    sw->setAnimation(animationEnabled);             // Sync to ScreenWriter
    cout << mediumSeparator << endl;
    if(animationEnabled) {
        cout << "Enabling animation mode..." << endl;  // Pixels now appear live as they are drawn
    }
    else {
        cout << "Disabling animation mode..." << endl; // Pixels appear all at once at the end
    }
    cout << mediumSeparator << endl;
    return animationEnabled; // Return new state so the menu can update its checkmark
}

bool AppManager::isAnimationEnabled() {
    return animationEnabled; // Simple getter
}

// ── Window handle getter ──────────────────────────────────────
HWND AppManager::getScreenOwner() {
    return this->hwnd; // Return the window handle (used by menu handlers that need it for dialogs, etc.)
}
