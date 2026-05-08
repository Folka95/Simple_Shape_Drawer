#ifndef ACTION_H
#define ACTION_H
#include "../algorithms/clipping_algorithm.h"
#include "../algorithms/drawing_algorithm.h"
#include "../algorithms/filling_algorithm.h"
#include "../core/shape.h"
#include "../rendering/screen_writer.h"

enum ActionType {
    ACTION_LEFT_CLICK,
    ACTION_RIGHT_CLICK,
    ACTION_MENU_SELECT
};

class Action {
protected:
    ActionType type;
    Action(ActionType type){
        this->type = type;
    }
    ~Action() {

    }
public:
    void applyAction() const ;
    ActionType getActionType() const {
        return type;
    }
};

class LeftClickAction : public Action {
public:
    LeftClickAction() : Action(ACTION_LEFT_CLICK){

    }
    ~LeftClickAction() {

    }
    void applyAction() const {

    }
};

// class Action {
// protected:
//     Shape *shape;
//     Action(const Shape &shape){
//         this->shape = shape.clone();
//     }
//     ~Action() {
//         delete shape;
//     }
// public:
//     void applyAction(ScreenWriter *sw);
//     Shape* getShape() const {
//         return shape->clone();
//     }
// };
//
// class DrawAction : public Action {
// private:
//     DrawingAlgorithm *drawingAlgorithm;
// public:
//     DrawAction(Shape *shape, DrawingAlgorithm *drawingAlgorithm) : Action(shape) {
//         this->drawingAlgorithm = drawingAlgorithm;
//     }
//     ~DrawAction() {
//         delete this->drawingAlgorithm;
//     }
//     void applyAction(ScreenWriter *sw) {
//         drawingAlgorithm->draw(*shape, sw);
//     }
// };
//
// class FillAction : public Action {
// private:
//     FillingAlgorithm *fillingAlgorithm;
//     Shape *clippingRegion;
//     Point startPoint;
// public:
//     FillAction(const Shape &shape, FillingAlgorithm *fillingAlgorithm, const Shape &clippingRegion, const Point &startPoint) : Action(shape) {
//         this->fillingAlgorithm = fillingAlgorithm;
//         this->clippingRegion = clippingRegion.clone();
//         this->startPoint = startPoint;
//     }
//     ~FillAction() {
//         delete this->fillingAlgorithm;
//         delete this->clippingRegion;
//     }
//     void applyAction(ScreenWriter *sw) const {
//         fillingAlgorithm->fill(*shape, *clippingRegion, startPoint, sw);
//     }
// };
//
// class ClipAction : public Action {
// private:
//     ClippingAlgorithm *clippingAlgorithm;
//     Shape *clippingRegion;
// public:
//     ClipAction(const Shape &shape, ClippingAlgorithm *clippingAlgorithm, const Shape &clippingRegion) : Action(shape) {
//         this->clippingAlgorithm = clippingAlgorithm;
//         this->clippingRegion = clippingRegion.clone();
//     }
//     ~ClipAction() {
//         delete this->clippingAlgorithm;
//         delete this->clippingRegion;
//     }
//     void applyAction(ScreenWriter *sw) const {
//         clippingAlgorithm->clip(*shape, *clippingRegion, sw);
//     }
// };

#endif //ACTION_H
