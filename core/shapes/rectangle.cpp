#include "rectangle.h"
using namespace std;

RectangleShape::RectangleShape() : Shape(
    SHAPE_RECTANGLE,
    "Rectangle\n"s +
    "How to draw ?\n" +
    "    2 Points\n" +
    "    1st point: Top Left\n" +
    "    2nd point: Bottom Right\n"
) {
}

void RectangleShape::initialize() {
    if (points.size() > 2) {
        return;
    }
    if (points.size() == 2) {
        width = points[1].x - points[0].x;
        height = points[1].y - points[0].y;
    }
}

bool RectangleShape::isInside(const Point &point) const {
    if (points.size() < 2) {
        return false;
    }
    return  point.x >= points[0].x && point.x <= points[1].x &&
            point.y >= points[0].y && point.y <= points[1].y;
}

bool RectangleShape::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "RectangleShape::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* RectangleShape::clone() const {
    RectangleShape* newRectangle = new RectangleShape();
    newRectangle->points = this->points;
    newRectangle->borderColor = this->borderColor;
    newRectangle->fillColor = this->fillColor;
    return newRectangle;
}

void RectangleShape::takeAction(int actionID) {

}