#pragma once
enum MenuID {
    // File Menu
    FILE_CLEAR,
    FILE_SAVE,
    FILE_LOAD,

    // Circle Algorithms
    CIRCLE_Direct,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE_Polar,
    CIRCLE_MIDPOINT,
    CIRCLE_MODIFIED_MIDPOINT,

    // Line Algorithms
    LINE_DDA,
    LINE_MIDPOINT,
    LINE_PARAMETRIC
};

HMENU createFileMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, MenuID::FILE_CLEAR, "Clear Screen");
    AppendMenu(hFileMenu, MF_STRING, MenuID::FILE_SAVE, "Save Screen");
    AppendMenu(hFileMenu, MF_STRING, MenuID::FILE_LOAD, "Load Screen");
    return hFileMenu;
}

HMENU createLineMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, LINE_DDA, "DDA");
    AppendMenu(hFileMenu, MF_STRING, LINE_MIDPOINT, "Midpoint");
    AppendMenu(hFileMenu, MF_STRING, LINE_PARAMETRIC, "Parametric");
    return hFileMenu;
}

HMENU createCircleMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, CIRCLE_Direct, "Direct");
    AppendMenu(hFileMenu, MF_STRING, CIRCLE_POLAR, "Polar");
    AppendMenu(hFileMenu, MF_STRING, CIRCLE_ITERATIVE_Polar, "Iterative Polar");
    AppendMenu(hFileMenu, MF_STRING, CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hFileMenu, MF_STRING, CIRCLE_MODIFIED_MIDPOINT, "Modified Midpoint");
    return hFileMenu;
}


void createAppMenu(HMENU hMenu) {
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFileMenu(), "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createLineMenu(), "Line");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createCircleMenu(), "Circle");
}