#include "app_manager.h"
#include <bits/stdc++.h>

#include "../algorithms/drawing/polygon/polygon_drawing_algorithm.h"
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
    shapeHistory.push_back(new PolygonShape<1>());
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
}

AppManager::~AppManager() {
    delete this->sw;
    delete this->drawingAlgorithm;
    delete this->fillingAlgorithm;
    delete this->clippingAlgorithm;
    delete this->clippingRegion;
    shapeHistory.clear();
    actionHistory.clear();
    shapeHistory.push_back(new PolygonShape<1>());
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
}

void AppManager::reset() {
    this->removeClippingAlgorithm();
    this->removeDrawingAlgorithm();
    this->removeFillingAlgorithm();
    sw->setBackgroundColor(RGB(0, 0, 0));
    sw->clearScreen();
    shapeHistory.clear();
    actionHistory.clear();
    shapeHistory.push_back(new PolygonShape<1>());
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
}

void AppManager::Private_setShape(Shape *shape, bool isUser) {
    if(!shapeHistory.empty() && !shapeHistory.back()->isEnoughToDraw()) {
        shapeHistory.pop_back();
    }
    shapeHistory.push_back(shape);
    if(isUser) {
        cout << mediumSeparator << endl;
        cout << shape->getDescription() << endl;
        cout << mediumSeparator << endl;
    }
}

void AppManager::setShape(Shape *shape) {
    Private_setShape(shape, true);
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
    if(this->drawingAlgorithm != nullptr) {
        delete this->drawingAlgorithm;
    }
    this->drawingAlgorithm = nullptr;
}

void AppManager::removeFillingAlgorithm() {
    if(this->fillingAlgorithm != nullptr) {
        delete this->fillingAlgorithm;
    }
    this->fillingAlgorithm = nullptr;
}

void AppManager::removeClippingAlgorithm() {
    if(this->clippingAlgorithm != nullptr) {
        delete this->clippingAlgorithm;
    }
    if(this->clippingRegion != nullptr) {
        delete this->clippingRegion;
    }
    if(this->clippingDrawingAlgorithm != nullptr) {
        delete this->clippingDrawingAlgorithm;
    }
    this->clippingAlgorithm = nullptr;
    this->clippingRegion = nullptr;
    this->clippingDrawingAlgorithm = nullptr;
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

void AppManager::Private_applyRightClick(short x, short y, bool isUser) {
    if(shapeHistory.empty()) {
        cerr << "AppManager::Private_applyRightClick: shapeHistory is empty" << endl;
        return;
    }
    if(shapeHistory.back()->getType() == SHAPE_CURVE) {
        Private_applyRightClickCurve(x, y, isUser);
        return;
    }
    if(fillingAlgorithm == nullptr) {
        cerr << "AppManager::Private_applyRightClick: fillingAlgorithm is null" << endl;
        return;
    }
    int actionRank = 1;
    vector< Shape* > tmpShapes = shapeHistory;
    stable_sort(tmpShapes.begin(), tmpShapes.end(), [&](const Shape *a, const Shape *b) {
       return a->getArea() < b->getArea();
    });
    for(Shape* shape : tmpShapes) {
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

void AppManager::applyRightClick(short x, short y) {
    Private_applyRightClick(x, y, true);
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


void AppManager::Private_applyMenuSelection(short choice, vector< short > data, bool isUser) {
    vector< short > result = selectMainMenu(choice, this, data);
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_LOAD) {
        return;
    }
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_HARD_SAVE) {
        return;
    }
    if(mainMenuDecoder(choice) == FILE_MENU && subMenuDecoder(choice) == FILE_SOFT_SAVE) {
        return;
    }
    vector< short > tmp = result;
    tmp.insert(tmp.begin(), choice);
    actionHistory.push_back(new MenuSelectAction(2, tmp));
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::applyMenuSelection(short choice) {
    Private_applyMenuSelection(choice, {}, true);
}

void AppManager::clearScreen() {
    sw->clearScreen();
    Shape *current = shapeHistory.back()->clone();
    this->shapeHistory.clear();
    shapeHistory.push_back(current);
}

void AppManager::hardSaveScreen(string filepath) {
    vector< Action* > tmp;
    for(Action *action : actionHistory) {
        tmp.push_back(action->clone());
    }
    if(!tmp.empty() && tmp.back()->getRank() < 2) {
        tmp.pop_back();
    }
    FileManager::saveActions(tmp, filepath);
}

void AppManager::softSaveScreen(string filepath) {
    vector< vector< COLORREF > > screen = sw->getScreen();
    FileManager::saveScreen(screen, filepath);
}

void AppManager::loadScreen(string filepath) {
    actionHistory.clear();
    shapeHistory.clear();
    if(filepath.substr(filepath.size() - 4, 4) == ".ssv") {
        vector< vector< COLORREF > > screen = FileManager::loadScreen(filepath);
        sw->setScreen(screen, true);
    }
    else if(filepath.substr(filepath.size() - 4, 4) == ".hsv"){
        vector< Action* > actions = FileManager::loadActions(filepath);
        this->reset();
        for(Action *action : actions) {
            if(action->getActionType() == ACTION_LEFT_CLICK) {
                this->Private_applyLeftClick(action->getData()[0], action->getData()[1], false);
            }
            else if(action->getActionType() == ACTION_RIGHT_CLICK) {
                this->Private_applyRightClick(action->getData()[0], action->getData()[1], false);
            }
            else if(action->getActionType() == ACTION_MENU_SELECT) {
                vector< short > tmp;
                for(int i = 1; i < action->getData().size(); i++) {
                    tmp.push_back(action->getData()[i]);
                }
                this->Private_applyMenuSelection(action->getData()[0], tmp, false);
            }
            else {
                cerr << "AppManager::loadScreen: unknown action type" << endl;
            }
        }
        this->sw->updateScreen();
    }
    else {
        cerr << "AppManager::loadScreen: invalid file extension" << endl;
        return;
    }
}


void AppManager::changeMouse() {
    static const LPCTSTR cursors[] = {
        IDC_ARROW,
        IDC_HAND,
        IDC_CROSS,
        IDC_SIZEALL
    };

    static bool seeded = false;
    if (!seeded) {
        std::srand((unsigned)std::time(nullptr));
        seeded = true;
    }

    int index = std::rand() % (sizeof(cursors) / sizeof(cursors[0]));
    mouse.setSystemCursor(cursors[index]);
}

void AppManager::applyMouseCursor() {
    mouse.apply();
}

HWND AppManager::getScreenOwner() {
    return this->hwnd;
}
