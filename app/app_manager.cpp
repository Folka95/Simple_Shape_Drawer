#include "app_manager.h"
#include <bits/stdc++.h>

#include "../algorithms/drawing/polygon/polygon_drawing_algorithm.h"
#include "../io/action.h"
#include "../io/file_manager.h"
#include "../rendering/menu_handler.h"

using namespace std;
AppManager::AppManager(HWND _hwnd) {
    this->hwnd = _hwnd;
    this->featureEnabled = false;
    this->sw = new ScreenWriter(_hwnd);
    sw->setAnimation(this->featureEnabled);
    this->drawingAlgorithm = nullptr;
    this->fillingAlgorithm = nullptr;
    this->clippingAlgorithm = nullptr;
    this->clippingRegion = nullptr;
    this->clippingDrawingAlgorithm = nullptr;
    borderColor = RGB(255, 255, 255);
    fillColor = RGB(255, 255, 255);
    shapeHistory.push_back(new PolygonShape<1>());
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
    this->undo.push(this->copyActionVector(this->actionHistory));
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
    this->clearRedo();
    this->clearUndo();
}

void AppManager::Private_reset(bool isUser) {
    this->removeClippingAlgorithm();
    this->removeDrawingAlgorithm();
    this->removeFillingAlgorithm();
    sw->setBackgroundColor(RGB(0, 0, 0));
    sw->clearScreen();
    shapeHistory.clear();
    actionHistory.clear();
    shapeHistory.push_back(new PolygonShape<1>());
    this->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
    if(isUser) {
        this->clearRedo();
        this->clearUndo();
        this->undo.push(this->copyActionVector(this->actionHistory));
    }
}


void AppManager::reset() {
    Private_reset(true);
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

void AppManager::setShape(Shape *shape, bool isUser) {
    Private_setShape(shape, isUser);
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

void AppManager::setClippingAlgorithm(ClippingAlgorithm *clippingAlgorithm, DrawingAlgorithm *clipDrawingAlgorithm, Shape *region, bool isUser) {
    this->removeClippingAlgorithm();
    this->clippingAlgorithm = clippingAlgorithm;
    this->clippingDrawingAlgorithm = clipDrawingAlgorithm;
    this->clippingRegion = region;
    if(isUser) {
        cout << largeSeparator << endl;
        cout << "# This is the clipping region ---- " << endl;
        cout << this->clippingRegion->getDescription() << endl;
        cout << largeSeparator << endl;
    }
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
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo();
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
        if(actionRank == 3) {
            this->undo.push(this->copyActionVector(this->actionHistory));
            this->clearRedo();
        }
        this->sw->updateScreen();
    }
}

void AppManager::applyRightClick(short x, short y) {
    Private_applyRightClick(x, y, true);
}

void AppManager::applyLeftClickClippingMode(short x, short y, bool isUser) {
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
    if(isUser && actionRank == 3) {
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo();
    }
}

void AppManager::applyLeftClickNoneClipping(short x, short y, bool isUser) {
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
    if(isUser && actionRank == 3) {
        this->undo.push(this->copyActionVector(this->actionHistory));
        this->clearRedo();
    }
}

void AppManager::Private_applyLeftClick(short x, short y, bool isUser) {
    if(clippingAlgorithm == nullptr) {
        applyLeftClickNoneClipping(x, y, isUser);
    }
    else {
        applyLeftClickClippingMode(x, y, isUser);
    }
    if(isUser) {
        this->sw->updateScreen();
    }
}

void AppManager::applyLeftClick(short x, short y) {
    Private_applyLeftClick(x, y, true);
}


void AppManager::Private_applyMenuSelection(short choice, vector< short > data, bool isUser) {
    vector< short > result = selectMainMenu(choice, this, data, isUser);
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
        if(mainMenuDecoder(choice) == FILE_MENU || mainMenuDecoder(choice) == PREFERENCES_MENU) {
            undo.push(this->copyActionVector(this->actionHistory));
            this->clearRedo();
        }
        this->sw->updateScreen();
    }
}

void AppManager::applyMenuSelection(short choice) {
    Private_applyMenuSelection(choice, {}, true);
}

void AppManager::clearScreen() {
    cout << mediumSeparator << endl;
    cout << "Clearing screen" << endl;
    cout << mediumSeparator << endl;
    sw->clearScreen();
    Shape *current = shapeHistory.back()->clone();
    this->shapeHistory.clear();
    current->clear();
    shapeHistory.push_back(current);
}

void AppManager::hardSaveScreen(string filepath) {
    if(filepath.empty()) {
        return;
    }
    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".hsv") {
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Hard saving screen to (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;
        vector< Action* > tmp;
        for(Action *action : actionHistory) {
            tmp.push_back(action->clone());
        }
        if(!tmp.empty() && tmp.back()->getRank() < 2) {
            tmp.pop_back();
        }
        FileManager::saveActions(tmp, filepath);
    }
    else {
        cerr << "AppManager::hardSaveScreen: filepath is invalid" << endl;
    }
}

void AppManager::softSaveScreen(string filepath) {
    if(filepath.empty()) {
        return;
    }
    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".ssv") {
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Soft saving screen to (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;
        vector< vector< COLORREF > > screen = sw->getScreen();
        FileManager::saveScreen(screen, filepath);
    }
    else {
        cerr << "AppManager::softSaveScreen: filepath is invalid" << endl;
    }
}

void AppManager::Private_applyActions(vector< Action* > actions, bool isUser) {
    actionHistory.clear();
    shapeHistory.clear();
    this->Private_reset(isUser);
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
            cerr << "AppManager::Private_applyActions: unknown action type" << endl;
        }
    }
    this->sw->updateScreen();
}

void AppManager::loadScreen(string filepath) {
    if(filepath.empty()) {
        return;
    }
    if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".ssv") {
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Loading soft save file (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;
        actionHistory.clear();
        shapeHistory.clear();
        this->reset();
        vector< vector< COLORREF > > screen = FileManager::loadScreen(filepath);
        sw->setScreen(screen, true);
    }
    else if(filepath.size() > 4 && filepath.substr(filepath.size() - 4, 4) == ".hsv"){
        cout << mediumSeparator << endl;
        filesystem::path p(filepath);
        cout << "Loading hard save file (" << p.filename().string() << ")" << endl;
        cout << mediumSeparator << endl;
        Private_applyActions(FileManager::loadActions(filepath), true);
    }
    else {
        cerr << "AppManager::loadScreen: invalid file extension" << endl;
        return;
    }
}

void AppManager::clearUndo() {
    while(!undo.empty()) {
        undo.pop();
    }
}

void AppManager::clearRedo() {
    while(!redo.empty()) {
        redo.pop();
    }
}

vector< Action* > AppManager::copyActionVector(vector< Action* > &actions) {
    vector< Action* > tmp;
    for(Action *action : actions) {
        tmp.push_back(action->clone());
    }
    return tmp;
}
void AppManager::undoStep() {
    if(undo.size() > 1) {
        redo.push(this->copyActionVector(undo.top()));
        undo.pop();
        cout << mediumSeparator << endl;
        cout << "Undoing last operation..." << endl;
        cout << mediumSeparator << endl;
    }
    // stack< vector< Action* > > tmp;
    // while(!redo.empty()) {
    //     tmp.push(this->copyActionVector(redo.top()));
    //     redo.pop();
    // }
    Private_applyActions(undo.top(), false);
    // while(!tmp.empty()) {
    //     redo.push(this->copyActionVector(tmp.top()));
    //     tmp.pop();
    // }
}

void AppManager::redoStep() {
    if(!redo.empty()) {
        undo.push(this->copyActionVector(redo.top()));
        redo.pop();
        cout << mediumSeparator << endl;
        cout << "Redoing last operation..." << endl;
        cout << mediumSeparator << endl;
    }
    Private_applyActions(undo.top(), false);
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

bool AppManager::toggleFeature() {
    featureEnabled = !featureEnabled;
    sw->setAnimation(featureEnabled);
    cout << mediumSeparator << endl;
    if(featureEnabled) {
        cout << "Enabling animation mode..." << endl;
    }
    else {
        cout << "Disabling animation mode..." << endl;
    }
    cout << mediumSeparator << endl;
    return featureEnabled;
}

bool AppManager::isFeatureEnabled() {
    return featureEnabled;
}

HWND AppManager::getScreenOwner() {
    return this->hwnd;
}
