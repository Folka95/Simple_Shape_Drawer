// ============================================================
// action.h — Defines the Action class hierarchy.
// Every user interaction (left-click, right-click, menu pick)
// is recorded as an Action object and pushed into a history
// list. This history list is what makes Undo / Redo work:
// to undo, the app resets and replays all actions except
// the last one; to redo, it replays one more.
// ============================================================

#ifndef ACTION_H  // Include guard — prevents this header from being processed twice
#define ACTION_H  // Define the guard symbol

// ── Enum: what kind of interaction was recorded ─────────────
enum ActionType {
    ACTION_LEFT_CLICK,   // User clicked the left mouse button (places a shape point)
    ACTION_RIGHT_CLICK,  // User clicked the right mouse button (fills a shape / finalizes a curve)
    ACTION_MENU_SELECT   // User chose something from a menu (change shape, algorithm, color, etc.)
};

// ── Base class for all recorded actions ─────────────────────
class Action {
private:
    short rank; // Priority level: 1 = insignificant (e.g., incomplete click), 2 = setting change, 3 = visible drawing
                // The rank is used to decide whether an action should create an undo checkpoint
protected:
    std::vector< short > data; // Payload — for clicks: {x, y}; for menu: {menuID, ...extra}
    ActionType type;           // Which of the three ActionType values this action is

    // Protected constructor — only subclasses can construct an Action
    Action(ActionType type, short rank, std::vector< short > data){
        this->type = type;   // Store what kind of action this is
        this->rank = rank;   // Store how important this action is
        this->data = data;   // Store the associated data (coordinates, menu IDs, etc.)
    }
public:
    virtual ~Action() = default; // Virtual destructor so deleting via a base pointer calls the right subclass destructor

    ActionType getActionType() const {
        return type; // Returns whether this action is a click, right-click, or menu selection
    }

    virtual Action* clone() = 0; // Pure virtual — every subclass must implement how to deep-copy itself
                                  // Required so the undo stack can store independent copies

    short getRank() const {
        return rank; // Returns the importance level (1/2/3) — used to filter which actions create undo checkpoints
    }

    std::vector< short > getData() {
        return data; // Returns the payload (x,y for clicks; menu ID list for menu actions)
    }
};

// ── Left-click action ────────────────────────────────────────
// Created every time the user left-clicks the canvas.
class LeftClickAction : public Action {
public:
    LeftClickAction(short rank, short x, short y)
        : Action(ACTION_LEFT_CLICK, rank, {x, y}) {
        // Calls base constructor; stores x and y as the data payload
        // rank comes from AppManager: 1 if the click didn't complete a shape, 3 if it drew something
    }

    Action* clone() {
        // Creates an independent copy of this action so the undo stack doesn't share pointers with the live history
        return new LeftClickAction(this->getRank(), this->getData()[0], this->getData()[1]);
    }
};

// ── Right-click action ───────────────────────────────────────
// Created every time the user right-clicks the canvas (to fill or finalize).
class RightClickAction : public Action {
public:
    RightClickAction(short rank, short x, short y)
        : Action(ACTION_RIGHT_CLICK, rank, {x, y}) {
        // Same structure as LeftClickAction; right-click coordinates are stored as data
    }

    Action *clone() {
        // Deep copy so the undo stack snapshot is independent of the live history list
        return new RightClickAction(this->getRank(), this->getData()[0], this->getData()[1]);
    }
};

// ── Menu-selection action ────────────────────────────────────
// Created when the user picks a menu item (shape, algorithm, color, etc.).
class MenuSelectAction : public Action {
public:
    MenuSelectAction(short rank, const std::vector< short > &data)
        : Action(ACTION_MENU_SELECT, rank, data) {
        // data[0] = the menu item ID (which menu was chosen)
        // data[1..] = any extra parameters returned by the menu handler (e.g., color values)
    }

    Action* clone() {
        // Deep copy — duplicates the entire data vector so the undo stack snapshot is independent
        return new MenuSelectAction(this->getRank(), this->getData());
    }
};

#endif // ACTION_H  // End of include guard
