#pragma once
#ifndef APP_MANAGER_H
#define APP_MANAGER_H
#include "../algorithms/clipping_algorithm.h"
#include "../rendering/screen_writer.h"
#include "../core/shape.h"
#include "../algorithms/drawing_algorithm.h"
#include "../algorithms/filling_algorithm.h"
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

    Shape *currentShape;

    vector< Shape* > history;

    Shape *clippingRegion;
    COLORREF borderColor;
    COLORREF fillColor;

    void applyLeftClickClippingMode(int x, int y);
    void applyLeftClickNoneClipping(int x, int y);
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

    void applyRightClick(int x, int y);
    void applyLeftClick(int x, int y);

    void clearScreen();
    void saveScreen();
    void loadScreen();

    void clippingMode();
};

#endif
