#pragma once

#pragma once

enum MenuID {
    // File Menu
    FILE_CLEAR,
    FILE_SAVE,
    FILE_LOAD,

    // Preferences Menu
    PREFERENCES_BACKGROUND_COLOR,
    PREFERENCES_MOUSE_SHAPE,
    PREFERENCES_BORDER_COLOR,
    PREFERENCES_FILL_COLOR,

    // Line Algorithms
    LINE_DDA,
    LINE_MIDPOINT,
    LINE_PARAMETRIC,

    // Circle Algorithms
    CIRCLE_Direct,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE_Polar,
    CIRCLE_MIDPOINT,
    CIRCLE_MODIFIED_MIDPOINT,

    // Ellipse Algorithms
    ELLIPSE_DIRECT,
    ELLIPSE_POLAR,
    ELLIPSE_MIDPOINT,

    // Curves
    CURVE_CARDINAL_SPLINE,

    // Filling
    FILL_CIRCLE_LINES,
    FILL_CIRCLE_CIRCLES,
    FILL_SQUARE_HERMITE,
    FILL_RECTANGLE_BEZIER,
    FILL_CONVEX,
    FILL_NON_CONVEX,
    FILL_FLOOD_RECURSIVE,
    FILL_FLOOD_NON_RECURSIVE,

    // Clipping (Rectangle)
    CLIP_RECT_POINT,
    CLIP_RECT_LINE,
    CLIP_RECT_POLYGON,

    // Clipping (Square)
    CLIP_SQUARE_POINT,
    CLIP_SQUARE_LINE,

    // Clipping (Circle)
    CLIP_CIRCLE_POINT,
    CLIP_CIRCLE_LINE,

    // Smile face
    SMILE_HAPPY_FACE,
    SMILE_SAD_FACE
};

HMENU createFileMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, FILE_CLEAR, "Clear Screen");
    AppendMenu(hSubMenu, MF_STRING, FILE_SAVE, "Save Screen");
    AppendMenu(hSubMenu, MF_STRING, FILE_LOAD, "Load Screen");
    return hSubMenu;
}

HMENU createPreferencesMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, PREFERENCES_BACKGROUND_COLOR, "Change background color");
    AppendMenu(hSubMenu, MF_STRING, PREFERENCES_BORDER_COLOR, "Change border color");
    AppendMenu(hSubMenu, MF_STRING, PREFERENCES_FILL_COLOR, "Change filling color");
    return hSubMenu;
}

HMENU createLineMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, LINE_DDA, "DDA");
    AppendMenu(hSubMenu, MF_STRING, LINE_MIDPOINT, "Midpoint");
    AppendMenu(hSubMenu, MF_STRING, LINE_PARAMETRIC, "Parametric");
    return hSubMenu;
}

HMENU createCircleMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, CIRCLE_Direct, "Direct");
    AppendMenu(hSubMenu, MF_STRING, CIRCLE_POLAR, "Polar");
    AppendMenu(hSubMenu, MF_STRING, CIRCLE_ITERATIVE_Polar, "Iterative Polar");
    AppendMenu(hSubMenu, MF_STRING, CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hSubMenu, MF_STRING, CIRCLE_MODIFIED_MIDPOINT, "Modified Midpoint");
    return hSubMenu;
}

HMENU createEllipseMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, ELLIPSE_DIRECT, "Direct");
    AppendMenu(hSubMenu, MF_STRING, ELLIPSE_POLAR, "Polar");
    AppendMenu(hSubMenu, MF_STRING, ELLIPSE_MIDPOINT, "Midpoint");
    return hSubMenu;
}

HMENU createCurvesMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, CURVE_CARDINAL_SPLINE, "Cardinal Spline");
    return hSubMenu;
}

HMENU createFillingMenu() {
    HMENU hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, FILL_CIRCLE_LINES, "Fill Circle (Lines)");
    AppendMenu(hSubMenu, MF_STRING, FILL_CIRCLE_CIRCLES, "Fill Circle (Circles)");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, FILL_SQUARE_HERMITE, "Fill Square (Hermite Vertical)");
    AppendMenu(hSubMenu, MF_STRING, FILL_RECTANGLE_BEZIER, "Fill Rectangle (Bezier Horizontal)");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, FILL_CONVEX, "Convex Fill");
    AppendMenu(hSubMenu, MF_STRING, FILL_NON_CONVEX, "Non-Convex Fill");

    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hSubMenu, MF_STRING, FILL_FLOOD_RECURSIVE, "Flood Fill Recursive");
    AppendMenu(hSubMenu, MF_STRING, FILL_FLOOD_NON_RECURSIVE, "Flood Fill Non-Recursive");

    return hSubMenu;
}

HMENU createClippingMenu() {
    HMENU hMain = CreatePopupMenu();

    // Rectangle submenu
    HMENU hRect = CreatePopupMenu();
    AppendMenu(hRect, MF_STRING, CLIP_RECT_POINT, "Point");
    AppendMenu(hRect, MF_STRING, CLIP_RECT_LINE, "Line");
    AppendMenu(hRect, MF_STRING, CLIP_RECT_POLYGON, "Polygon");

    // Square submenu
    HMENU hSquare = CreatePopupMenu();
    AppendMenu(hSquare, MF_STRING, CLIP_SQUARE_POINT, "Point");
    AppendMenu(hSquare, MF_STRING, CLIP_SQUARE_LINE, "Line");

    // Circle submenu
    HMENU hCirc = CreatePopupMenu();
    AppendMenu(hCirc, MF_STRING, CLIP_CIRCLE_POINT, "Point");
    AppendMenu(hCirc, MF_STRING, CLIP_CIRCLE_LINE, "Line");

    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hRect, "Rectangle");
    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hSquare, "Square");
    AppendMenu(hMain, MF_POPUP, (UINT_PTR)hCirc, "Circle (Bonus)");


    return hMain;
}


HMENU createSmileFaceMenu() {
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, SMILE_HAPPY_FACE, "Happy Face");
    AppendMenu(hSubMenu, MF_STRING, SMILE_SAD_FACE, "Sad Face");
    return hSubMenu;
}

void createAppMenu(HMENU hMenu) {
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