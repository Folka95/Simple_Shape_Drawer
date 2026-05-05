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

class AppManager {
private:
    HWND hwnd;
    ScreenWriter *sw;

    DrawingAlgorithm *drawingAlgorithm;
    FillingAlgorithm *fillingAlgorithm;
    ClippingAlgorithm *clippingAlgorithm;

    vector< Shape* > history;

    Shape *clippingRegion;
    COLORREF borderColor;
    COLORREF fillColor;
public:
    AppManager(HWND _hwnd);
    ~AppManager();

    void setShape(Shape *shape);
    void setBoarderColor(COLORREF color);
    void setFillColor(COLORREF color);
    void setBackgroundColor(COLORREF color);

    void setFillingAlgorithm(FillingAlgorithm *fillingAlgorithm);
    void setDrawingAlgorithm(DrawingAlgorithm *drawingAlgorithm);
    void setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm);

    void applyRightClick(int x, int y);
    void applyLeftClick(int x, int y);

    void clearScreen();
    void saveScreen();
    void loadScreen();

    void clippingMode();
};

#endif
