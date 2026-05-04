#include "app_manager.h"

AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;
    this->sw = new ScreenWriter(_hwnd);
}

AppManager::~AppManager() {
    delete this->sw;
}

void AppManager::setShape(Shape *shape) {

}

void AppManager::setBoarderColor(COLORREF color) {

}

void AppManager::setFillColor(COLORREF color) {

}

void AppManager::setBackgroundColor(COLORREF color) {
    sw->setBackgroundColor(color);
}

void AppManager::applyRightClick(int x, int y) {
}

void AppManager::applyLeftClick(int x, int y) {
    // if(currentShape->addPoint(x, y) == DRAW_ME) {
    //     if(clippingRegion == nullptr) {
    //         currentDrawingAlgorithm->draw(currentShape, borderColor);
    //         currentShape->drawn();
    //     }
    //     else {
    //         currentDrawingAlgorithm->draw(currentShape, clippingRegion, borderColor);
    //         currentShape->drawn();
    //     }
    // }
}

void AppManager::clearScreen() {
    sw->clearScreen();
}

void AppManager::saveScreen() {

}

void AppManager::loadScreen() {

}
