#include "rectangle.h"

Rectangle::Rectangle() {
    type = SHAPE_RECTANGLE;
    description = "Rectangle\n";
    description += "How to draw ?\n";
    description += "    2 Points\n";
    description += "    1st point: Top Left\n";
    description += "    2nd point: Bottom Right\n";
}

void Rectangle::initialize() {
    if (points.size() > 2) {
        return;
    }
    if (points.size() == 2) {
        width = points[1].x - points[0].x;
        height = points[1].y - points[0].y;
    }
}

bool Rectangle::isInside(const Point &point) const {
    if (points.size() < 2) {
        return false;
    }
    return  point.x >= points[0].x && point.x <= points[1].x &&
            point.y >= points[0].y && point.y <= points[1].y;
}

bool Rectangle::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "Rectangle::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* Rectangle::clone() const {
    Rectangle* newRectangle = new Rectangle();
    newRectangle->points = this->points;
    newRectangle->borderColor = this->borderColor;
    newRectangle->fillColor = this->fillColor;
    return newRectangle;
}

void Rectangle::takeAction(int actionID) {

}