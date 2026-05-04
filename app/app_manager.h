#ifndef APP_MANAGER_H
#define APP_MANAGER_H
#include "../rendering/screen_writer.h"
#include "../core/shape.h"
#include "../algorithms/drawing_algorithm.h"
#include "../algorithms/filling_algorithm.h"

class Element {
    Shape shape;
    COLORREF borderColor;
    COLORREF fillColor;
};

class AppManager {
private:
    HWND hwnd;
    ScreenWriter *sw;
    Shape *currentShape;
    DrawingAlgorithm *currentDrawingAlgorithm;
    FillingAlgorithm *currentFillingAlgorithm;
    vector< Element > history;
public:
    AppManager(HWND _hwnd);
    ~AppManager();

    void setShape(Shape *shape);
    void setBoarderColor(COLORREF color);
    void setFillColor(COLORREF color);
    void setBackgroundColor(COLORREF color);

    void applyRightClick(int x, int y);
    void applyLeftClick(int x, int y) {
        if(currentShape->addPoint(Point(x, y)) == draw) {
            currentDrawingAlgorithm->draw(*currentShape);
        }
        else if(currentShape->addPoint(Point(x, y)) == fill) {
            currentFillingAlgorithm->fill(*currentShape);
        }
    }

    void clearScreen();
    void saveScreen();
    void loadScreen();
};

#endif
