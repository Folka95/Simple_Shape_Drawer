#include "app_manager.h"
#include <bits/stdc++.h>
#include "../io/action.h"
#include "../io/file_manager.h"

using namespace std;
AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;
    this->sw = new ScreenWriter(_hwnd);
    this->drawingAlgorithm = nullptr;
    this->fillingAlgorithm = nullptr;
    this->clippingAlgorithm = nullptr;
    this->clippingRegion = nullptr;
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
    shapeHistory.clear();
    actionHistory.clear();
}

void AppManager::setShape(Shape *shape) {
    if(!shapeHistory.empty() && !shapeHistory.back()->isEnoughToDraw()) {
        shapeHistory.pop_back();
    }
    shapeHistory.push_back(shape);
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
    this->drawingAlgorithm = nullptr;
}

void AppManager::removeFillingAlgorithm() {
    delete this->fillingAlgorithm;
    this->fillingAlgorithm = nullptr;
}

void AppManager::removeClippingAlgorithm() {
    delete this->clippingAlgorithm;
    delete this->clippingRegion;
    delete this->clippingDrawingAlgorithm;
    this->clippingAlgorithm = nullptr;
    this->clippingRegion = nullptr;
    this->clippingDrawingAlgorithm = nullptr;
}

void AppManager::applyRightClick(short x, short y) {
    if(shapeHistory.empty()) {
        cerr << "AppManager::applyLeftClick: shapeHistory is empty" << endl;
        return;
    }
    if(fillingAlgorithm == nullptr) {
        cerr << "AppManager::applyRightClick: fillingAlgorithm is null" << endl;
        return;
    }
    int actionRank = 1;
    for(Shape* shape : shapeHistory) {
        if(shape->isEnoughToDraw() && shape->isInside(Point(x, y))) {
            shape->fillColor = fillColor;
            fillingAlgorithm->fill(*shape, *clippingRegion, Point(x, y), sw);
            actionRank = 3;
            break;
        }
    }
    actionHistory.push_back(new RightClickAction(actionRank, x, y));
    this->sw->updateScreen();
}

void AppManager::applyLeftClickClippingMode(short x, short y) {
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
    int actionRank = 1;
    if(this->clippingRegion->isEnoughToDraw()) {
        if(shapeHistory.back()->isEnoughToDraw()) {
            shapeHistory.push_back(shapeHistory.back()->clone());
            shapeHistory.back()->clear();
        }
        if(shapeHistory.back()->addPoint(Point(x, y))) {
            shapeHistory.back()->borderColor = borderColor;
            clippingAlgorithm->clip(*shapeHistory.back(), *this->clippingRegion, sw);
            actionRank = 3;
        }
    }
    else {
        if(this->clippingRegion->addPoint(Point(x, y))) {
            this->clippingRegion->borderColor = borderColor;
            this->clippingDrawingAlgorithm->draw(*this->clippingRegion, sw);
            actionRank = 3;
        }
    }
    actionHistory.push_back(new LeftClickAction(actionRank, x, y));
}

void AppManager::applyLeftClickNoneClipping(short x, short y) {
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
    if(shapeHistory.back()->isEnoughToDraw()) {
        shapeHistory.push_back(shapeHistory.back()->clone());
        shapeHistory.back()->clear();
    }
    int actionRank = 1;
    if(shapeHistory.back()->addPoint(Point(x, y))) {
        shapeHistory.back()->borderColor = borderColor;
        drawingAlgorithm->draw(*shapeHistory.back(), sw);
        actionRank = 3;
    }
    actionHistory.push_back(new LeftClickAction(actionRank, x, y));
}

void AppManager::applyLeftClick(short x, short y) {
    if(clippingAlgorithm == nullptr) {
        applyLeftClickNoneClipping(x, y);
    }
    else {
        applyLeftClickClippingMode(x, y);
    }
    this->sw->updateScreen();
}

void AppManager::applyMenuSelection(short choice) {
    actionHistory.push_back(new MenuSelectAction(2, choice));
    this->sw->updateScreen();
}

void AppManager::clearScreen() {
    sw->clearScreen();
}

void AppManager::hardSaveScreen() {
    vector< Action* > tmp;
    for(Action *action : actionHistory) {
        tmp.push_back(action->clone());
    }
    if(!tmp.empty() && tmp.back()->getRank() < 3) {
        tmp.pop_back();
    }
    FileManager::saveActions(tmp, "outH.sdv");
}

void AppManager::softSaveScreen() {
    vector< vector< COLORREF > > screen = sw->getScreen();
    FileManager::saveScreen(screen, "outS.sdv");
}

void AppManager::loadScreen() {
    actionHistory.clear();
    shapeHistory.clear();
    vector< vector< COLORREF > > screen = FileManager::loadScreen("outS.sdv");
    sw->setScreen(screen);
}

void AppManager::undoStep() {

}

void AppManager::redoStep() {

}

HWND AppManager::getScreenOwner() {
    return this->hwnd;
}
