enum MenuID {
    FILE_CLEAR,
    FILE_EXIT,
    CIRCLE_DDA,
    CIRCLE_MIDPOINT,
    CIRCLE_PARAMETRIC
};

HMENU createFileMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, MenuID::FILE_CLEAR, "Clear");
    AppendMenu(hFileMenu, MF_STRING, MenuID::FILE_EXIT, "Exit");
    return hFileMenu;
}

HMENU createLineMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_DDA, "DDA");
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_PARAMETRIC, "Parametric");
    return hFileMenu;
}

HMENU createCircleMenu() {
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_DDA, "DDA");
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hFileMenu, MF_STRING, MenuID::CIRCLE_PARAMETRIC, "Parametric");
    return hFileMenu;
}


void createAppMenu(HMENU hMenu) {
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createFileMenu(), "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)createLineMenu(), "Line");
}