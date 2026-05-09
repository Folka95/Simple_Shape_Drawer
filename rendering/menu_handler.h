#pragma once
#include <bits/stdc++.h>
#include <Windows.h>
#include "../app/app_manager.h"
#include "../io/input_reader.h"
#include "../core/shapes/line.h"
#include "../core/shapes/circle.h"
#include "../core/shapes/rectangle.h"
#include "../core/shapes/ellipse.h"
#include "../algorithms/line/line_dda_drawing_algorithm.h"
#include "../algorithms/line/line_parametric_drawing_algorithm.h"
#include "../algorithms/line/line_midpoint_drawing_algorithm.h"
#include "../algorithms/circle/circle_polar_drawing_algorithm.h"
#include "../algorithms/circle/circle_iterative_polar_drawing_algorithm.h"
#include "../algorithms/circle/circle_direct_drawing_algorithm.h"
#include "../algorithms/circle/circle_midpoint_drawing_algorithm.h"
#include "../algorithms/circle/circle_midpoint_fast_drawing_algorithm.h"
#include "../algorithms/ellipse/ellipse_direct_drawing_algorithm.h"
#include "../algorithms/ellipse/ellipse_Polar_drawing_algorithm.h"
#include "../algorithms/ellipse/ellipse_midpoint_drawing_algorithm.h"
#include "../algorithms/clipping/circle/circle_line_clipping_algorithm.h"
#include "../algorithms/clipping/circle/circle_point_clipping_algorithm.h"
#include "../algorithms/filling/iterive_flood_fill_filling_algorithm.h"
#include "../algorithms/filling/flood_fill_filling_algorithm.h"
#include "../core/shapes/polygon.h"


enum Menu {
    FILE_MENU,
    PREFERENCES_MENU,
    LINE_MENU,
    CIRCLE_MENU,
    ELLIPSE_MENU,
    CURVE_MENU,
    FILL_MENU,
    CLIP_MENU,
    SMILE_MENU
};

enum FileMenu {
    FILE_CLEAR,
    FILE_SOFT_SAVE,
    FILE_HARD_SAVE,
    FILE_LOAD
};

enum PreferencesMenu {
    PREFERENCES_BACKGROUND_COLOR,
    PREFERENCES_MOUSE_SHAPE,
    PREFERENCES_BORDER_COLOR,
    PREFERENCES_FILL_COLOR
};

enum LineMenu {
    LINE_DDA,
    LINE_MIDPOINT,
    LINE_PARAMETRIC
};

enum CircleMenu {
    CIRCLE_DIRECT,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE_POLAR,
    CIRCLE_MIDPOINT,
    CIRCLE_MODIFIED_MIDPOINT
};

enum EllipseMenu {
    ELLIPSE_DIRECT,
    ELLIPSE_POLAR,
    ELLIPSE_MIDPOINT
};

enum CurveMenu {
    CURVE_CARDINAL_SPLINE
};

enum FillMenu {
    FILL_CIRCLE_LINES,
    FILL_CIRCLE_CIRCLES,
    FILL_SQUARE_HERMITE,
    FILL_RECTANGLE_BEZIER,
    FILL_CONVEX,
    FILL_NON_CONVEX,
    FILL_FLOOD_RECURSIVE,
    FILL_FLOOD_NON_RECURSIVE
};

enum ClipMenu {
    CLIP_RECT_POINT,
    CLIP_RECT_LINE,
    CLIP_RECT_POLYGON,

    CLIP_SQUARE_POINT,
    CLIP_SQUARE_LINE,

    CLIP_CIRCLE_POINT,
    CLIP_CIRCLE_LINE
};

enum SmileMenu {
    SMILE_HAPPY_FACE,
    SMILE_SAD_FACE
};

const int MAX_ENUM_SIZE = 20;


inline short enumEncoder(short menu, short submenu) {
    return menu * MAX_ENUM_SIZE + submenu;
}

inline short mainMenuDecoder(short value) {
    return value / MAX_ENUM_SIZE;
}

inline short subMenuDecoder(short value) {
    return value % MAX_ENUM_SIZE;
}

inline HMENU createFileMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_CLEAR), "Clear Screen");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_SOFT_SAVE), "Soft Save Screen");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_HARD_SAVE), "Hard Save Screen");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_LOAD), "Load Screen");
    return hSubMenu;
}

inline HMENU createPreferencesMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_BACKGROUND_COLOR), "Change background color");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_BORDER_COLOR), "Change border color");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_FILL_COLOR), "Change filling color");
    return hSubMenu;
}

inline HMENU createLineMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(LINE_MENU, LINE_DDA), "DDA");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(LINE_MENU, LINE_MIDPOINT), "Midpoint");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(LINE_MENU, LINE_PARAMETRIC), "Parametric");
    return hSubMenu;
}

inline HMENU createCircleMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_DIRECT), "Direct");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_POLAR), "Polar");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_ITERATIVE_POLAR), "Iterative Polar");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_MIDPOINT), "Midpoint");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_MODIFIED_MIDPOINT), "Modified Midpoint");
    return hSubMenu;
}

inline HMENU createEllipseMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(ELLIPSE_MENU, ELLIPSE_DIRECT), "Direct");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(ELLIPSE_MENU, ELLIPSE_POLAR), "Polar");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(ELLIPSE_MENU, ELLIPSE_MIDPOINT), "Midpoint");
    return hSubMenu;
}

inline HMENU createCurvesMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CURVE_MENU, CURVE_CARDINAL_SPLINE), "Cardinal Spline");
    return hSubMenu;
}

inline HMENU createFillingMenu() {
    HMENU hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_CIRCLE_LINES), "Fill Circle (Lines)");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_CIRCLE_CIRCLES), "Fill Circle (Circles)");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_SQUARE_HERMITE), "Fill Square (Hermite Vertical)");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_RECTANGLE_BEZIER), "Fill Rectangle (Bezier Horizontal)");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_CONVEX), "Convex Fill");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_NON_CONVEX), "Non-Convex Fill");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_FLOOD_RECURSIVE), "Flood Fill Recursive");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILL_MENU, FILL_FLOOD_NON_RECURSIVE), "Flood Fill Non-Recursive");

    return hSubMenu;
}

inline HMENU createClippingMenu() {
    HMENU hMain = CreatePopupMenu();

    HMENU hRect = CreatePopupMenu();
    AppendMenu(hRect, MF_STRING, enumEncoder(CLIP_MENU, CLIP_RECT_POINT), "Point");
    AppendMenu(hRect, MF_STRING, enumEncoder(CLIP_MENU, CLIP_RECT_LINE), "Line");
    AppendMenu(hRect, MF_STRING, enumEncoder(CLIP_MENU, CLIP_RECT_POLYGON), "Polygon");

    HMENU hSquare = CreatePopupMenu();
    AppendMenu(hSquare, MF_STRING, enumEncoder(CLIP_MENU, CLIP_SQUARE_POINT), "Point");
    AppendMenu(hSquare, MF_STRING, enumEncoder(CLIP_MENU, CLIP_SQUARE_LINE), "Line");

    HMENU hCirc = CreatePopupMenu();
    AppendMenu(hCirc, MF_STRING, enumEncoder(CLIP_MENU, CLIP_CIRCLE_POINT), "Point");
    AppendMenu(hCirc, MF_STRING, enumEncoder(CLIP_MENU, CLIP_CIRCLE_LINE), "Line");

    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hRect, "Rectangle");
    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hSquare, "Square");
    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hCirc, "Circle (Bonus)");

    return hMain;
}

inline HMENU createSmileFaceMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(SMILE_MENU, SMILE_HAPPY_FACE), "Happy Face");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(SMILE_MENU, SMILE_SAD_FACE), "Sad Face");
    return hSubMenu;
}

inline void createAppMenu(HMENU hMenu) {
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFileMenu(), "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createPreferencesMenu(), "Preferences");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createLineMenu(), "Line");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createCircleMenu(), "Circle");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createEllipseMenu(), "Ellipse");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createCurvesMenu(), "Curves");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFillingMenu(), "Filling");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createClippingMenu(), "Clipping");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createSmileFaceMenu(), "Smile Face");
}

inline void selectFileMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        case FILE_CLEAR:
            appManager->clearScreen();
            break;

        case FILE_SOFT_SAVE:
            appManager->softSaveScreen();
            break;

        case FILE_HARD_SAVE:
            appManager->hardSaveScreen();
            break;

        case FILE_LOAD:
            appManager->loadScreen();
            break;

        default:
            std::cerr << "selectFileMenu: Unknown File Menu value: " << value << '\n';
            break;
    }
}

inline void selectPreferencesMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        case PREFERENCES_MOUSE_SHAPE:
            break;

        case PREFERENCES_BACKGROUND_COLOR:
            appManager->setBackgroundColor(pickColor(appManager->getScreenOwner()));
            break;

        case PREFERENCES_BORDER_COLOR:
            appManager->setBoarderColor(pickColor(appManager->getScreenOwner()));
            break;

        case PREFERENCES_FILL_COLOR:
            appManager->setFillColor(pickColor(appManager->getScreenOwner()));
            break;

        default:
            std::cerr << "selectPreferencesMenu: Unknown Preferences Menu value: " << value << '\n';
            break;
    }
}

inline void selectLineMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        case LINE_DDA:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_DDA_DrawingAlgorithm());
            break;

        case LINE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_Midpoint_DrawingAlgorithm());
            break;

        case LINE_PARAMETRIC:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_Parametric_DrawingAlgorithm());
            break;

        default:
            std::cerr << "selectLineMenu: Unknown Line Menu value: " << value << '\n';
            break;
    }
}

inline void selectCircleMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        case CIRCLE_DIRECT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_Direct_DrawingAlgorithm());
            break;

        case CIRCLE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_Polar_DrawingAlgorithm());
            break;

        case CIRCLE_ITERATIVE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_iterativePolar_DrawingAlgorithm());
            break;

        case CIRCLE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_MidPoint_DrawingAlgorithm());
            break;
    
        case CIRCLE_MODIFIED_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_MidPoint_Fast_DrawingAlgorithm());
            break;

        default:
            std::cerr << "selectCircleMenu: Unknown Circle Menu value: " << value << '\n';
            break;
    }
}

inline void selectEllipseMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        case ELLIPSE_DIRECT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_Direct_DrawingAlgorithm());
            break;
        
        case ELLIPSE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_Polar_DrawingAlgorithm());
            break;
        
        case ELLIPSE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_MidPoint_DrawingAlgorithm());
            break;

        default:
            std::cerr << "selectEllipseMenu: Unknown Ellipse Menu value: " << value << '\n';
            break;
    }
}

inline void selectCurvesMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        // case CURVE_CARDINAL_SPLINE:
        //     appManager->setDrawingAlgorithm(new Cardinal_Spline_DrawingAlgorithm());
        //     break;

        default:
            std::cerr << "selectCurvesMenu: Unknown Curves Menu value: " << value << '\n';
            break;
    }
}

inline void selectFillingMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        // case FILL_CIRCLE_LINES:
        //     appManager->setFillingAlgorithm(new FillCircleLines_FillingAlgorithm());
        //     break;
        //
        // case FILL_CIRCLE_CIRCLES:
        //     appManager->setFillingAlgorithm(new FillCircleCircles_FillingAlgorithm());
        //     break;
        //
        // case FILL_SQUARE_HERMITE:
        //     appManager->setFillingAlgorithm(new FillSquareHermite_FillingAlgorithm());
        //     break;
        //
        // case FILL_RECTANGLE_BEZIER:
        //     appManager->setFillingAlgorithm(new FillRectangleBezier_FillingAlgorithm());
        //     break;
        //
        // case FILL_CONVEX:
        //     appManager->setFillingAlgorithm(new Convex_FillingAlgorithm());
        //     break;
        //
        // case FILL_NON_CONVEX:
        //     appManager->setFillingAlgorithm(new NonConvex_FillingAlgorithm());
        //     break;

        case FILL_FLOOD_RECURSIVE:
            appManager->setFillingAlgorithm(new FloodFill_FillingAlgorithm());
            break;

        case FILL_FLOOD_NON_RECURSIVE:
            appManager->setFillingAlgorithm(new IterFloodFill_FillingAlgorithm());
            break;

        default:
            std::cerr << "selectFillingMenu: Unknown Filling Menu value: " << value << '\n';
            break;
    }
}

inline void selectClippingMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        // case CLIP_RECT_POINT:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_RECT_LINE:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_RECT_POLYGON:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_SQUARE_POINT:
        //     appManager->setClippingAlgorithm();
        //     break;
        //
        // case CLIP_SQUARE_LINE:
        //     appManager->setClippingAlgorithm();
        //     break;

        case CLIP_CIRCLE_POINT:
            appManager->removeDrawingAlgorithm();
            appManager->setClippingAlgorithm(
                new Circle_Point_ClippingAlgorithm(),
                new Circle_Polar_DrawingAlgorithm(),
                new Circle()
            );
            appManager->setShape(new PolygonShape<1>());
            break;

        case CLIP_CIRCLE_LINE:
            appManager->removeDrawingAlgorithm();
            appManager->setClippingAlgorithm(
                new Circle_Line_ClippingAlgorithm(),
                new Circle_Polar_DrawingAlgorithm(),
                new Circle()
            );
            appManager->setShape(new Line());
            break;

        default:
            std::cerr << "selectClippingMenu: Unknown Clipping Menu value: " << value << '\n';
            break;
    }
}

inline void selectSmileMenu(short value, AppManager *appManager) {
    switch (subMenuDecoder(value)) {
        // case SMILE_HAPPY_FACE:
        //     appManager->setShape();
        //     break;
        //
        // case SMILE_SAD_FACE:
        //     appManager->setShape();
        //     break;
        default:
            std::cerr << "selectSmileMenu: Unknown Smile Menu value: " << value << '\n';
            break;
    }
}

inline void selectMainMenu(short value, AppManager *appManager) {
    switch (mainMenuDecoder(value)) {
        case FILE_MENU:
            selectFileMenu(value, appManager);
            break;

        case PREFERENCES_MENU:
            selectPreferencesMenu(value, appManager);
            break;

        case LINE_MENU:
            selectLineMenu(value, appManager);
            break;

        case CIRCLE_MENU:
            selectCircleMenu(value, appManager);
            break;

        case ELLIPSE_MENU:
            selectEllipseMenu(value, appManager);
            break;

        case CURVE_MENU:
            selectCurvesMenu(value, appManager);
            break;

        case FILL_MENU:
            selectFillingMenu(value, appManager);
            break;

        case CLIP_MENU:
            selectClippingMenu(value, appManager);
            break;

        case SMILE_MENU:
            selectSmileMenu(value, appManager);
            break;

        default:
            std::cerr << "selectMainMenu: Unknown Main Menu value: " << value << '\n';
            break;
    }
}

inline void selectMenu(short value, AppManager *appManager) {
    selectMainMenu(value, appManager);
}
