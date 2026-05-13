#pragma once
#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "../algorithms/clipping_algorithm.h"
#include "../rendering/screen_writer.h"
#include "../core/shape.h"
#include "../algorithms/drawing_algorithm.h"
#include "../algorithms/filling_algorithm.h"
#include "../io/action.h"

#include <vector>
#include <stack>
using namespace std;

#define largeSeparator string(30, '=')
#define mediumSeparator string(20, '=')
#define smallSeparator string(10, '=')
#include "../rendering/mouse_cursor_manager.h"

class AppManager {
private:
    HWND hwnd;
    ScreenWriter *sw;
    MouseCursorManager mouse;


    DrawingAlgorithm *drawingAlgorithm;
    FillingAlgorithm *fillingAlgorithm;
    ClippingAlgorithm *clippingAlgorithm;
    DrawingAlgorithm *clippingDrawingAlgorithm;

    vector< Action* > actionHistory;
    vector< Shape * > shapeHistory;

    stack< vector< Action* > > undo, redo;

    Shape *clippingRegion;
    COLORREF borderColor;
    COLORREF fillColor;

    void applyLeftClickClippingMode(short x, short y, bool isUser);
    void applyLeftClickNoneClipping(short x, short y, bool isUser);
    void reset();

    bool featureEnabled;

    void Private_applyLeftClick(short x, short y, bool isUser);
    void Private_applyRightClick(short x, short y, bool isUser);
    void Private_applyRightClickCurve(short x, short y, bool isUser);
    void Private_applyMenuSelection(short choice, vector< short > data, bool isUser);
    void Private_setShape(Shape *shape, bool isUser);
    void Private_applyActions(vector< Action* > actions, bool isUser);
    void Private_reset(bool isUser);

    void clearUndo();
    void clearRedo();
    vector< Action* > copyActionVector(vector< Action* > &actions);
public:
    AppManager(HWND _hwnd);
    ~AppManager();

    void setShape(Shape *shape, bool isUser = true);
    void setBoarderColor(COLORREF color);
    void setFillColor(COLORREF color);
    void setBackgroundColor(COLORREF color);

    void setDrawingAlgorithm(DrawingAlgorithm *drawingAlgorithm);
    void setFillingAlgorithm(FillingAlgorithm *fillingAlgorithm);
    void setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm, DrawingAlgorithm *clipDrawingAlgorithm, Shape *region);

    void removeDrawingAlgorithm();
    void removeFillingAlgorithm();
    void removeClippingAlgorithm();

    void applyRightClick(short x, short y);
    void applyLeftClick(short x, short y);
    void applyMenuSelection(short choice);

    void changeMouse();
    void applyMouseCursor();

    bool toggleFeature();
    bool isFeatureEnabled();

    void clearScreen();
    void softSaveScreen(string filepath);
    void hardSaveScreen(string filepath);
    void loadScreen(string filepath);

    void undoStep();
    void redoStep();


    HWND getScreenOwner();
};

#endif
