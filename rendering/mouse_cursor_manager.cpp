#include "mouse_cursor_manager.h"

MouseCursorManager::MouseCursorManager() {
    this->defaultCursor = LoadCursor(nullptr, IDC_ARROW);
    this->currentCursor = this->defaultCursor;
}

void MouseCursorManager::setCursor(HCURSOR cursor) {
    if (cursor != nullptr) {
        currentCursor = cursor;
    }
    apply();
}

void MouseCursorManager::setSystemCursor(LPCTSTR cursorId) {
    HCURSOR cursor = LoadCursor(nullptr, cursorId);
    if (cursor != nullptr) {
        currentCursor = cursor;
    }
    apply();
}

void MouseCursorManager::reset() {
    currentCursor = defaultCursor;
    apply();
}

void MouseCursorManager::apply() {
    if (currentCursor != nullptr) {
        SetCursor(currentCursor);
    }
}

HCURSOR MouseCursorManager::getCurrentCursor() const {
    return currentCursor;
}
