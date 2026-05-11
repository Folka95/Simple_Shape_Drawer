#pragma once
#ifndef MOUSE_CURSOR_MANAGER_H
#define MOUSE_CURSOR_MANAGER_H
#include <windows.h>

class MouseCursorManager {
private:
    HCURSOR defaultCursor;
    HCURSOR currentCursor;

public:
    explicit MouseCursorManager();

    void setCursor(HCURSOR cursor);
    void setSystemCursor(LPCTSTR cursorId);
    void reset();
    void apply();

    HCURSOR getCurrentCursor() const;
};

#endif
