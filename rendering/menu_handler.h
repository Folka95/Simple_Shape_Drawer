#pragma once
#include <bits/stdc++.h>
#include <Windows.h>
#include "../app/app_manager.h"
#include "../io/input_reader.h"
#include "../core/shapes/line.h"
#include "../core/shapes/circle.h"
#include "../core/shapes/rectangle.h"
#include "../core/shapes/ellipse.h"
#include "../core/shapes/curve.h"
#include "../algorithms/drawing/line/line_dda_drawing_algorithm.h"
#include "../algorithms/drawing/line/line_parametric_drawing_algorithm.h"
#include "../algorithms/drawing/line/line_midpoint_drawing_algorithm.h"
#include "../algorithms/drawing/circle/circle_polar_drawing_algorithm.h"
#include "../algorithms/drawing/circle/circle_iterative_polar_drawing_algorithm.h"
#include "../algorithms/drawing/circle/circle_direct_drawing_algorithm.h"
#include "../algorithms/drawing/circle/circle_midpoint_drawing_algorithm.h"
#include "../algorithms/drawing/circle/circle_midpoint_fast_drawing_algorithm.h"
#include "../algorithms/drawing/ellipse/ellipse_direct_drawing_algorithm.h"
#include "../algorithms/drawing/ellipse/ellipse_Polar_drawing_algorithm.h"
#include "../algorithms/drawing/ellipse/ellipse_midpoint_drawing_algorithm.h"
#include "../algorithms/drawing/curve/curve_spline_drawing_algorithm.h"
#include "../algorithms/clipping/circle/circle_line_clipping_algorithm.h"
#include "../algorithms/clipping/circle/circle_point_clipping_algorithm.h"
#include "../algorithms/clipping/rectangle/rectangle_line_clipping_algorithm.h"
#include "../algorithms/clipping/rectangle/rectangle_point_clipping_algorithm.h"
#include "../algorithms/clipping/rectangle/rectangle_polygon_clipping_algorithm.h"
#include "../algorithms/clipping/square/square_line_clipping_algorithm.h"
#include "../algorithms/clipping/square/square_point_clipping_algorithm.h"
#include "../algorithms/drawing/polygon/polygon_drawing_algorithm.h"
#include "../algorithms/drawing/rectangle/rectangle_drawing_algorithm.h"
#include "../algorithms/drawing/smile_face/happy_smile_face_drawing_algorithm.h"
#include "../algorithms/drawing/smile_face/sad_smile_face_drawing_algorithm.h"
#include "../algorithms/drawing/square/square_drawing_algorithm.h"
#include "../algorithms/filling/circle/circle_quarter_with_circle_filling_algorithm.h"
#include "../algorithms/filling/circle/circle_quarter_with_line_filling_algorithm.h"
#include "../algorithms/filling/floodfill/iterive_flood_fill_filling_algorithm.h"
#include "../algorithms/filling/floodfill/flood_fill_filling_algorithm.h"
#include "../algorithms/filling/curve/fill_with_curve_filling_algorithm.h"
#include "../algorithms/filling/linescan/convex_filling_algorithm.h"
#include "../algorithms/filling/linescan/non_convex_filling_algorithm.h"
#include "../core/shapes/happy_smile_face.h"
#include "../core/shapes/polygon.h"
#include "../core/shapes/sad_smile_face.h"
#include "../core/shapes/square.h"

enum Menu {
    FILE_MENU,
    PREFERENCES_MENU,
    LINE_MENU,
    CIRCLE_MENU,
    ELLIPSE_MENU,
    CURVE_MENU,
    FILL_MENU,
    CLIP_MENU,
    SMILE_MENU,
    OTHER_MENU
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
    PREFERENCES_FILL_COLOR,
    PREFERENCES_TOGGLE_FEATURE
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

enum OtherMenu {
    OTHER_SQUARE,
    OTHER_RECTANGLE,
    OTHER_POLYGON
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
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_SOFT_SAVE), "Soft Save Screen");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_HARD_SAVE), "Hard Save Screen");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(FILE_MENU, FILE_LOAD), "Load Screen");
    return hSubMenu;
}

inline HMENU createPreferencesMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_BACKGROUND_COLOR), "Change background color");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_BORDER_COLOR), "Change border color");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_FILL_COLOR), "Change filling color");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(PREFERENCES_MENU, PREFERENCES_MOUSE_SHAPE), "Change mouse randomly");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING | (false ? MF_CHECKED : MF_UNCHECKED), enumEncoder(PREFERENCES_MENU, PREFERENCES_TOGGLE_FEATURE), "Enable Animation");
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
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_POLAR), "Polar");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(CIRCLE_MENU, CIRCLE_ITERATIVE_POLAR), "Iterative Polar");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

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

inline HMENU createOtherShapeMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(OTHER_MENU, OTHER_SQUARE), "Square");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(OTHER_MENU, OTHER_RECTANGLE), "Rectangle");
    AppendMenu(hSubMenu, MF_STRING, enumEncoder(OTHER_MENU, OTHER_POLYGON), "Polygon");
    return hSubMenu;
}

inline void createAppMenu(HMENU hMenu) {
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFileMenu(), "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createPreferencesMenu(), "Preferences");

    AppendMenu(hMenu, MF_POPUP | MF_DISABLED, 0, "|");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createLineMenu(), "Line");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createCircleMenu(), "Circle");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createEllipseMenu(), "Ellipse");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createCurvesMenu(), "Curves");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createSmileFaceMenu(), "Smile Face");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createOtherShapeMenu(), "Other Shapes");

    AppendMenu(hMenu, MF_POPUP | MF_DISABLED, 0, "|");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFillingMenu(), "Filling");

    AppendMenu(hMenu, MF_POPUP | MF_DISABLED, 0, "|");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createClippingMenu(), "Clipping");
}

inline vector< short > selectFileMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case FILE_CLEAR:
            appManager->clearScreen();
            return {};

        case FILE_SOFT_SAVE:{
            string filepath = InputReader::userSaveFileDialog(appManager->getScreenOwner(), "image", "ssv", "Save File");
            appManager->softSaveScreen(filepath);
            return {};
        }

        case FILE_HARD_SAVE: {
            string filepath = InputReader::userSaveFileDialog(appManager->getScreenOwner(), "image", "hsv", "Save File");
            appManager->hardSaveScreen(filepath);
            return {};
        }
        case FILE_LOAD: {
            string filepath = InputReader::userGetFileDialog(appManager->getScreenOwner(), {"ssv", "hsv"}, "Save File");
            appManager->loadScreen(filepath);
            return {};
        }
        default:
            std::cerr << "selectFileMenu: Unknown File Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectPreferencesMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case PREFERENCES_MOUSE_SHAPE:
            appManager->changeMouse();
            return {};

        case PREFERENCES_BACKGROUND_COLOR: {
            COLORREF color = InputReader::userPickColor(appManager->getScreenOwner(), memoriezedData);
            appManager->setBackgroundColor(color);
            return {GetRValue(color), GetGValue(color), GetBValue(color)};
        }

        case PREFERENCES_BORDER_COLOR:{
            COLORREF color = InputReader::userPickColor(appManager->getScreenOwner(), memoriezedData);
            appManager->setBoarderColor(color);
            return {GetRValue(color), GetGValue(color), GetBValue(color)};
        }
        case PREFERENCES_FILL_COLOR:{
            COLORREF color = InputReader::userPickColor(appManager->getScreenOwner(), memoriezedData);
            appManager->setFillColor(color);
            return {GetRValue(color), GetGValue(color), GetBValue(color)};
        }

        case PREFERENCES_TOGGLE_FEATURE: {
            bool isEnabled = appManager->toggleFeature();
            HMENU hMenu = GetMenu(appManager->getScreenOwner());
            CheckMenuItem(hMenu, value, MF_BYCOMMAND | (isEnabled ? MF_CHECKED : MF_UNCHECKED));
            return {};
        }

        default:
            std::cerr << "selectPreferencesMenu: Unknown Preferences Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectLineMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case LINE_DDA:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_DDA_DrawingAlgorithm());
            return {};

        case LINE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_Midpoint_DrawingAlgorithm());
            return {};

        case LINE_PARAMETRIC:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setDrawingAlgorithm(new Line_Parametric_DrawingAlgorithm());
            return {};

        default:
            std::cerr << "selectLineMenu: Unknown Line Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectCircleMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case CIRCLE_DIRECT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_Direct_DrawingAlgorithm());
            return {};

        case CIRCLE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_Polar_DrawingAlgorithm());
            return {};

        case CIRCLE_ITERATIVE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_iterativePolar_DrawingAlgorithm());
            return {};

        case CIRCLE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_MidPoint_DrawingAlgorithm());
            return {};

        case CIRCLE_MODIFIED_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Circle());
            appManager->setDrawingAlgorithm(new Circle_MidPoint_Fast_DrawingAlgorithm());
            return {};

        default:
            std::cerr << "selectCircleMenu: Unknown Circle Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectEllipseMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case ELLIPSE_DIRECT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_Direct_DrawingAlgorithm());
            return {};

        case ELLIPSE_POLAR:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_Polar_DrawingAlgorithm());
            return {};

        case ELLIPSE_MIDPOINT:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new EllipseShape());
            appManager->setDrawingAlgorithm(new Ellipse_MidPoint_DrawingAlgorithm());
            return {};

        default:
            std::cerr << "selectEllipseMenu: Unknown Ellipse Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectCurvesMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case CURVE_CARDINAL_SPLINE: {
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            short Cvalue = (short)InputReader::userPickRange(appManager->getScreenOwner(), 0, 100, 50, "Smoothness Percent %", memoriezedData);
            appManager->setShape(new CurveShape());
            appManager->setDrawingAlgorithm(new Curve_Spline_DrawingAlgorithm(Cvalue));
            return {Cvalue};
        }
        default:
            std::cerr << "selectCurvesMenu: Unknown Curves Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectFillingMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case FILL_CIRCLE_LINES:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new Circle_QuarterWithLine_FillingAlgorithm());
            return {};

        case FILL_CIRCLE_CIRCLES:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new Circle_QuarterWithCircle_FillingAlgorithm());
            return {};

        case FILL_SQUARE_HERMITE:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new FillWithCurveFillingAlgorithm());
            return {};

        case FILL_RECTANGLE_BEZIER:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new FillWithCurveFillingAlgorithm());
            return {};
        
        case FILL_CONVEX:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new ConvexFillingAlgorithm());
            return {};
        
        case FILL_NON_CONVEX:
            appManager->removeClippingAlgorithm();
            appManager->setFillingAlgorithm(new NonConvexFillingAlgorithm());
            return {};

        case FILL_FLOOD_RECURSIVE:
            appManager->setFillingAlgorithm(new FloodFill_FillingAlgorithm());
            return {};

        case FILL_FLOOD_NON_RECURSIVE:
            appManager->setFillingAlgorithm(new IterFloodFill_FillingAlgorithm());
            return {};

        default:
            std::cerr << "selectFillingMenu: Unknown Filling Menu value: " << value << '\n';
            return {};
    }
    return {};
}



inline vector< short > selectClippingMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
         case CLIP_RECT_POINT:
             appManager->removeDrawingAlgorithm();
             appManager->removeFillingAlgorithm();
             appManager->setShape(new PolygonShape<1>());
             appManager->setClippingAlgorithm(
                     new Rectangle_Point_ClippingAlgorithm(),
                     new Rectangle_DrawingAlgorithm(),
                     new RectangleShape()
                     );

             return {};
         case CLIP_RECT_LINE:
             appManager->removeDrawingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setClippingAlgorithm(
                    new Rectangle_Line_ClippingAlgorithm(),
                    new Rectangle_DrawingAlgorithm(),
                    new RectangleShape()
            );
             return {};
        case CLIP_RECT_POLYGON: {
            appManager->removeDrawingAlgorithm();
            appManager->removeFillingAlgorithm();
            pair< Shape *, short > res = InputReader::userReadPolygon(appManager, memoriezedData);
            appManager->setShape(res.first);
            appManager->setClippingAlgorithm(
                new Rectangle_Polygon_ClippingAlgorithm(),
                new Rectangle_DrawingAlgorithm(),
                new RectangleShape());
            return {res.second};
        }

        case CLIP_SQUARE_POINT:
            appManager->removeDrawingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new PolygonShape<1>());
            appManager->setClippingAlgorithm(
                new Square_Point_ClippingAlgorithm(),
                new Square_DrawingAlgorithm(),
                new Square());
            return {};

        case CLIP_SQUARE_LINE:
            appManager->removeDrawingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Line());
            appManager->setClippingAlgorithm(
                new Square_Line_ClippingAlgorithm(),
                new Square_DrawingAlgorithm(),
                new Square());
            return {};

        case CLIP_CIRCLE_POINT:
            appManager->removeDrawingAlgorithm();
            appManager->setClippingAlgorithm(
                new Circle_Point_ClippingAlgorithm(),
                new Circle_Polar_DrawingAlgorithm(),
                new Circle()
            );
            appManager->setShape(new PolygonShape<1>());
            return {};

        case CLIP_CIRCLE_LINE:
            appManager->removeDrawingAlgorithm();
            appManager->setClippingAlgorithm(
                new Circle_Line_ClippingAlgorithm(),
                new Circle_Polar_DrawingAlgorithm(),
                new Circle()
            );
            appManager->setShape(new Line());
            return {};

        default:
            std::cerr << "selectClippingMenu: Unknown Clipping Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectSmileMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case SMILE_HAPPY_FACE:
            appManager->removeClippingAlgorithm();
            appManager->setShape(new HappySmileFace());
            appManager->setDrawingAlgorithm(new HappySmileFace_DrawingAlgorithm());
            return {};

        case SMILE_SAD_FACE:
            appManager->removeClippingAlgorithm();
            appManager->setShape(new SadSmileFace());
            appManager->setDrawingAlgorithm(new SadSmileFace_DrawingAlgorithm());
            return {};
        default:
            std::cerr << "selectSmileMenu: Unknown Smile Menu value: " << value << '\n';
        return {};
    }
    return {};
}

inline vector< short > selectOtherMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (subMenuDecoder(value)) {
        case OTHER_SQUARE:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new Square());
            appManager->setDrawingAlgorithm(new Square_DrawingAlgorithm());
            return {};

        case OTHER_RECTANGLE:
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            appManager->setShape(new RectangleShape());
            appManager->setDrawingAlgorithm(new Rectangle_DrawingAlgorithm());
            return {};
        case OTHER_POLYGON: {
            appManager->removeClippingAlgorithm();
            appManager->removeFillingAlgorithm();
            pair< Shape *, short > res = InputReader::userReadPolygon(appManager, memoriezedData);
            appManager->setShape(res.first);
            appManager->setDrawingAlgorithm(new Polygon_DrawingAlgorithm());
            return {res.second};
        }
        default:
            std::cerr << "selectSmileMenu: Unknown Smile Menu value: " << value << '\n';
            return {};
    }
    return {};
}

inline vector< short > selectMainMenu(short value, AppManager *appManager, const vector< short > &memoriezedData) {
    switch (mainMenuDecoder(value)) {
        case FILE_MENU:
            return selectFileMenu(value, appManager, memoriezedData);


        case PREFERENCES_MENU:
            return selectPreferencesMenu(value, appManager, memoriezedData);

        case LINE_MENU:
            return selectLineMenu(value, appManager, memoriezedData);

        case CIRCLE_MENU:
            return selectCircleMenu(value, appManager, memoriezedData);

        case ELLIPSE_MENU:
            return selectEllipseMenu(value, appManager, memoriezedData);

        case CURVE_MENU:
            return selectCurvesMenu(value, appManager, memoriezedData);

        case FILL_MENU:
            return selectFillingMenu(value, appManager, memoriezedData);

        case CLIP_MENU:
            return selectClippingMenu(value, appManager, memoriezedData);

        case SMILE_MENU:
            return selectSmileMenu(value, appManager, memoriezedData);

        case OTHER_MENU:
            return selectOtherMenu(value, appManager, memoriezedData);

        default:
            std::cerr << "selectMainMenu: Unknown Main Menu value: " << value << '\n';
            return {};
    }
    return {};
}
