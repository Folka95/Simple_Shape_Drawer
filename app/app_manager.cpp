#include "app_manager.h"
#include <bits/stdc++.h>
#include "../io/action.h"
#include "../io/file_manager.h"
#include "../rendering/menu_handler.h"

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

void AppManager::reset() {
    delete this->drawingAlgorithm;
    delete this->fillingAlgorithm;
    delete this->clippingAlgorithm;
    delete this->clippingRegion;
    shapeHistory.clear();
    actionHistory.clear();
    sw->setBackgroundColor(RGB(0, 0, 0));
    sw->clearScreen();
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

void AppManager::Private_applyRightClick(short x, short y, bool isUser) {
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
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::Private_applyRightClickCurve(short x, short y, bool isUser) {
    if(clippingRegion != nullptr) {
        cerr << "AppManager::Private_applyRightClickCurve: clippingRegion is NOT null" << endl;
        return;
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
    this->shapeHistory.back()->takeAction(1);
    if(!shapeHistory.back()->isEnoughToDraw()) {
        cerr << "AppManager::Private_applyRightClickCurve: not enough to draw" << endl;
        return;
    }
    shapeHistory.back()->borderColor = borderColor;
    drawingAlgorithm->draw(*shapeHistory.back(), sw);
    actionHistory.push_back(new RightClickAction(3, x, y));
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::applyRightClick(short x, short y) {
    if(this->shapeHistory.back()->getType() == SHAPE_CURVE) {
        Private_applyRightClickCurve(x, y, true);
    }
    else {
        Private_applyRightClick(x, y, true);
    }
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

void AppManager::Private_applyLeftClick(short x, short y, bool isUser) {
    if(clippingAlgorithm == nullptr) {
        applyLeftClickNoneClipping(x, y);
    }
    else {
        applyLeftClickClippingMode(x, y);
    }
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::applyLeftClick(short x, short y) {
    Private_applyLeftClick(x, y, true);
}


void AppManager::Private_applyMenuSelection(short choice, bool isUser) {
    selectMainMenu(choice, this);
    if(subMenuDecoder(choice) != FileMenu::FILE_LOAD) {
        actionHistory.push_back(new MenuSelectAction(2, choice));
    }
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::applyMenuSelection(short choice) {
    Private_applyMenuSelection(choice, true);
}

void AppManager::clearScreen() {
    sw->clearScreen();
    this->shapeHistory.clear();
}

void AppManager::hardSaveScreen() {
    vector< Action* > tmp;
    for(Action *action : actionHistory) {
        tmp.push_back(action->clone());
    }
    if(!tmp.empty() && tmp.back()->getRank() < 3) {
        tmp.pop_back();
    }
    FileManager::saveActions(tmp, "out.hsv");
}

void AppManager::softSaveScreen() {
    vector< vector< COLORREF > > screen = sw->getScreen();
    FileManager::saveScreen(screen, "out.ssv");
}

void AppManager::loadScreen() {
    actionHistory.clear();
    shapeHistory.clear();
    string filename ="out.ssv";
    if(filename.substr(filename.size() - 4, 4) == ".ssv") {
        vector< vector< COLORREF > > screen = FileManager::loadScreen(filename);
        sw->setScreen(screen, true);
    }
    else {
        vector< Action* > actions = FileManager::loadActions(filename);
        this->reset();
        for(Action *action : actions) {
            if(action->getActionType() == ACTION_LEFT_CLICK) {
                this->Private_applyLeftClick(action->getData()[0], action->getData()[1], false);
            }
            else if(action->getActionType() == ACTION_RIGHT_CLICK) {
                this->Private_applyRightClick(action->getData()[0], action->getData()[1], false);
            }
            else if(action->getActionType() == ACTION_MENU_SELECT) {
                this->Private_applyMenuSelection(action->getData()[0], false);
            }
            else {
                cerr << "AppManager::loadScreen: unknown action type" << endl;
            }
        }
        this->sw->updateScreen();
    }
}

void AppManager::undoStep() {

}

void AppManager::redoStep() {

}

HWND AppManager::getScreenOwner() {
    return this->hwnd;
}
