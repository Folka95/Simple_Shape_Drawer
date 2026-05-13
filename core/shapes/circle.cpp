#include "circle.h"
using namespace std;

Circle::Circle() : Shape(SHAPE_CIRCLE, "Circle",
    "Shape Type : Circle\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the center and a point on the circumference of the circle\n"
    ) {

}

void Circle::initialize() {
    this->radius = points[0].euclideanDistance(points[1]);
    this->center = points[0];
    this->area = M_PI * this->radius * this->radius;
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
    newCircle->initialize();
    return newCircle;
}


std::vector< Point > Circle::getSidePoints() const {
    std::vector< Point > res;
    if (this->radius <= 0) return res;
    double step = 1.0 / this->radius;
    for (double radian = 0; radian < 2 * M_PI; radian += step) {
        double x = (this->center.x + this->radius * cos(radian));
        double y = (this->center.y + this->radius * sin(radian));
        res.push_back(Point(x, y));
    }
    return res;
}

double Circle::getRadius() const {
    return this->radius;
}

Point Circle::getCenter() const {
    return this->center;
}

int Circle::scanQuarter(const Point &point) const {
    double dx = points[0].dx(point);
    double dy = points[0].dy(point);

    // edge case
    if (dx == 0 || dy == 0) {
        return 0;
    }

    if (dx > 0 && dy > 0) {
        return 1;
    } else if (dx < 0 && dy > 0) {
        return 2;
    } else if (dx < 0 && dy < 0) {
        return 3;
    } else if (dx > 0 && dy < 0) {
        return 4;
    }
    // unexpected behavior
    return 5;
}
