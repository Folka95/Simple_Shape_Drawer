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
using namespace std;

#define largeSeparator string(30, '=')
#define mediumSeparator string(20, '=')
#define smallSeparator string(10, '=')

class AppManager {
private:
    HWND hwnd;
    ScreenWriter *sw;

    DrawingAlgorithm *drawingAlgorithm;
    FillingAlgorithm *fillingAlgorithm;
    ClippingAlgorithm *clippingAlgorithm;
    DrawingAlgorithm *clippingDrawingAlgorithm;

    vector< Action* > actionHistory;
    vector< Shape * > shapeHistory;

    Shape *clippingRegion;
    COLORREF borderColor;
    COLORREF fillColor;

    void applyLeftClickClippingMode(short x, short y);
    void applyLeftClickNoneClipping(short x, short y);
    void reset();

    void Private_applyLeftClick(short x, short y, bool isUser);
    void Private_applyRightClick(short x, short y, bool isUser);
    void Private_applyRightClickCurve(short x, short y, bool isUser);
    void Private_applyMenuSelection(short choice, bool isUser);
public:
    AppManager(HWND _hwnd);
    ~AppManager();

    void setShape(Shape *shape);
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

    void undoStep();
    void redoStep();

    void clearScreen();
    void softSaveScreen();
    void hardSaveScreen();
    void loadScreen();

    HWND getScreenOwner();
};

#endif
