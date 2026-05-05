#include "app_manager.h"
#include <bits/stdc++.h>
using namespace std;
AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;
    this->sw = new ScreenWriter(_hwnd);
}

AppManager::~AppManager() {
    delete this->sw;
}

void AppManager::setShape(Shape *shape) {
    if(!history.empty() && !history.back()->isEnoughToDraw()) {
        history.pop_back();
    }
    history.push_back(shape);
    cout << string(20, '=') << endl;
    cout << shape->getDescription() << endl;
    cout << string(20, '=') << endl;
}

void AppManager::setBoarderColor(COLORREF color) {
    borderColor = color;
}

void AppManager::setFillColor(COLORREF color) {
    fillColor = color;
}

void AppManager::setBackgroundColor(COLORREF color) {
    sw->setBackgroundColor(color);
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
    if(history.back()->isEnoughToDraw()) {
        history.back()->fillColor = fillColor;
        fillingAlgorithm->fill(*history.back());
    }
}

void AppManager::applyLeftClick(int x, int y) {
    if(history.empty()) {
        cerr << "AppManager::applyLeftClick: history is empty" << endl;
        return;
    }
    if(history.back()->isEnoughToDraw()) {
        history.push_back(history.back()->clone());
        history.back()->clear();
    }
    Point p(x, y);
    if(history.back()->addPoint(p)) {
        if(clippingRegion == nullptr) {
            if(drawingAlgorithm == nullptr) {
                cerr << "AppManager::applyLeftClick: drawingAlgorithm is null" << endl;
                return;
            }
            history.back()->borderColor = borderColor;
            drawingAlgorithm->draw(*history.back());
        }
        else {
            if(clippingAlgorithm == nullptr) {
                cerr << "AppManager::applyLeftClick: clippingAlgorithm is null" << endl;
                return;
            }
            history.back()->borderColor = borderColor;
            clippingAlgorithm->clip(*history.back(), *clippingRegion);
        }
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


