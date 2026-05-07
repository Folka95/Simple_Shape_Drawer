#include "app_manager.h"
#include <bits/stdc++.h>
using namespace std;
AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;
    this->sw = new ScreenWriter(_hwnd);
    this->drawingAlgorithm = nullptr;
    this->fillingAlgorithm = nullptr;
    this->clippingAlgorithm = nullptr;
    this->clippingRegion = nullptr;
    this->currentShape = nullptr;
    this->clippingDrawingAlgorithm = nullptr;
    borderColor = RGB(255, 255, 255);
    fillColor = RGB(255, 255, 255);
}

AppManager::~AppManager() {
    delete this->sw;
    delete this->drawingAlgorithm;
    delete this->fillingAlgorithm;
    delete this->clippingAlgorithm;
    delete this->clippingRegion;
    history.clear();
}

void AppManager::setShape(Shape *shape) {
    if(!history.empty() && !history.back()->isEnoughToDraw()) {
        history.pop_back();
    }
    currentShape = shape->clone();
    history.push_back(shape);
    cout << mediumSeparator << endl;
    cout << shape->getDescription() << endl;
    cout << mediumSeparator << endl;
}

void AppManager::setBoarderColor(COLORREF color) {
    borderColor = color;
}

void AppManager::setFillColor(COLORREF color) {
    fillColor = color;
}

void AppManager::setBackgroundColor(COLORREF color) {
    sw->changeBackgroundColor(color);
}

void AppManager::setFillingAlgorithm(FillingAlgorithm *fillingAlgorithm) {
    this->removeFillingAlgorithm();
    this->fillingAlgorithm = fillingAlgorithm;
}

void AppManager::setDrawingAlgorithm(DrawingAlgorithm *drawingAlgorithm) {
    this->removeDrawingAlgorithm();
    this->drawingAlgorithm = drawingAlgorithm;
}

void AppManager::setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm, DrawingAlgorithm *clipDrawingAlgorithm, Shape *region) {
    this->removeClippingAlgorithm();
    this->clippingAlgorithm = clippingAlgorithm;
    this->clippingDrawingAlgorithm = clipDrawingAlgorithm;
    this->clippingRegion = region;
    cout << largeSeparator << endl;
    cout << "# This is the clipping region ---- " << endl;
    cout << this->clippingRegion->getDescription() << endl;
    cout << largeSeparator << endl;
}

void AppManager::removeDrawingAlgorithm() {
    delete this->drawingAlgorithm;
}

void AppManager::removeFillingAlgorithm() {
    delete this->fillingAlgorithm;
}

void AppManager::removeClippingAlgorithm() {
    delete this->clippingAlgorithm;
    delete this->clippingRegion;
    delete this->clippingDrawingAlgorithm;
}

void AppManager::applyRightClick(int x, int y) {
    if(history.empty()) {
        cerr << "AppManager::applyRightClick: history is empty" << endl;
        return;
    }
    if(fillingAlgorithm == nullptr) {
        cerr << "AppManager::applyRightClick: fillingAlgorithm is null" << endl;
        return;
    }
    for(Shape* shape : history) {
        if(shape->isEnoughToDraw() && shape->isInside(Point(x, y))) {
            shape->fillColor = fillColor;
            fillingAlgorithm->fill(*shape, *clippingRegion, Point(x, y), sw);
        }
    }
    // if(history.back()->isEnoughToDraw() && history.back()->isInside(Point(x, y))) {
    //     history.back()->fillColor = fillColor;
    //     fillingAlgorithm->fill(*history.back(), *clippingRegion, sw);
    // }
}

void AppManager::applyLeftClickClippingMode(int x, int y) {
    if(clippingRegion == nullptr) {
        cerr << "AppManager::applyLeftClickClippingMode: clippingAlgorithm is null" << endl;
        return;
    }
    if(this->clippingRegion->isEnoughToDraw()) {
        if(history.empty()) {
            cerr << "AppManager::applyLeftClickClippingMode: history is empty" << endl;
            return;
        }
        if(history.back()->isEnoughToDraw()) {
            history.push_back(currentShape->clone());
            history.back()->clear();
        }
        Point p(x, y);
        if(history.back()->addPoint(p)) {
            if(this->clippingAlgorithm == nullptr) {
                cerr << "AppManager::applyLeftClickClippingMode: clippingAlgorithm is null" << endl;
                return;
            }
            history.back()->borderColor = borderColor;
            clippingAlgorithm->clip(*history.back(), *this->clippingRegion, sw);
        }
    }
    else {
        if(this->clippingRegion->addPoint(Point(x, y))) {
            if(this->clippingDrawingAlgorithm == nullptr) {
                cerr << "AppManager::applyLeftClickClippingMode: clippingDrawingAlgorithm is null" << endl;
                return;
            }
            this->clippingRegion->borderColor = borderColor;
            this->clippingDrawingAlgorithm->draw(*this->clippingRegion, sw);
        }
    }
}

void AppManager::applyLeftClickNoneClipping(int x, int y) {
    if(clippingRegion != nullptr) {
        cerr << "AppManager::applyLeftClickNoneClipping: clippingAlgorithm is NOT null" << endl;
        return;
    }
    if(history.empty()) {
        cerr << "AppManager::applyLeftClick: history is empty" << endl;
        return;
    }
    if(history.back()->isEnoughToDraw()) {
        history.push_back(currentShape->clone());
        history.back()->clear();
    }
    Point p(x, y);
    if(history.back()->addPoint(p)) {
        if(drawingAlgorithm == nullptr) {
            cerr << "AppManager::applyLeftClickNoneClipping: drawingAlgorithm is null" << endl;
            return;
        }
        history.back()->borderColor = borderColor;
        drawingAlgorithm->draw(*history.back(), sw);
    }
}

void AppManager::applyLeftClick(int x, int y) {
    if(clippingAlgorithm == nullptr) {
        applyLeftClickNoneClipping(x, y);
    }
    else {
        applyLeftClickClippingMode(x, y);
    }
}

void AppManager::clearScreen() {
    sw->clearScreen();
}

void AppManager::saveScreen() {
    if(!history.empty() && !history.back()->isEnoughToDraw()) {
        history.pop_back();
    }
}

void AppManager::loadScreen() {
    history.clear();
}
