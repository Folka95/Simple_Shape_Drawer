#include "circle.h"

Circle::Circle() : Shape() {
    type = SHAPE_CIRCLE;
    description = "";
    description += "Shape Type : Circle\n";
    description += "How to draw ?\n";
    description += "    Use mouse left-click to make 2 clicks represents \n";
    description += "    the center and a point on the circumference of the circle\n";
}

void Circle::initialize() {
    this->radius = points[0].euclideanDistance(points[1]);
}

bool Circle::isInside(const Point &point) const {
    double lhs = (point.x - points[0].x) * (point.x - points[0].x) + (point.y - points[0].y) * (point.y - points[0].y);
    double rhs = radius * radius;
    return lhs <= rhs;
}

bool Circle::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "Circle::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* Circle::clone() const {
    Circle* newCircle = new Circle();
    newCircle->points = this->points;
    newCircle->borderColor = this->borderColor;
    newCircle->fillColor = this->fillColor;
    return newCircle;
}

void Circle::takeAction(int actionID) {

}
