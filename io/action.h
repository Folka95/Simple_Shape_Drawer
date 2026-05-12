#ifndef ACTION_H
#define ACTION_H

enum ActionType {
    ACTION_LEFT_CLICK,
    ACTION_RIGHT_CLICK,
    ACTION_MENU_SELECT
};

class Action {
private:
    short rank;
protected:
    std::vector< short > data;
    ActionType type;
    Action(ActionType type, short rank, std::vector< short > data){
        this->type = type;
        this->rank = rank;
        this->data = data;
    }
public:
    virtual ~Action() = default;
    ActionType getActionType() const {
        return type;
    }
    virtual Action* clone() = 0;
    short getRank() const {
        return rank;
    }
    std::vector< short > getData() {
        return data;
    }
};

class LeftClickAction : public Action {
public:
    LeftClickAction(short rank, short x, short y) : Action(ACTION_LEFT_CLICK, rank, {x, y}){

    }
    Action* clone() {
        return new LeftClickAction(this->getRank(), this->getData()[0], this->getData()[1]);
    }
};

class RightClickAction : public Action {
public:
    RightClickAction(short rank, short x, short y) : Action(ACTION_RIGHT_CLICK, rank, {x, y}){

    }
    Action *clone() {
        return new RightClickAction(this->getRank(), this->getData()[0], this->getData()[1]);
    }
};

class MenuSelectAction : public Action {
public:
    MenuSelectAction(short rank, const std::vector< short > &data) : Action(ACTION_MENU_SELECT, rank, data){

    }
    Action* clone() {
        return new MenuSelectAction(this->getRank(), this->getData());
    }
};

#endif
