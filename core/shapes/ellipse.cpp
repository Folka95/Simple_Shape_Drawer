#include "ellipse.h"

EllipseShape::EllipseShape() : Shape() {
    type = SHAPE_ELLIPSE;
    description = "";
    description += "Shape Type : Ellipse\n";
    description += "How to draw ?\n";
    description += "    Use mouse left-click to make 3 clicks represents \n";
    description += "    the center and two points on the circumference of the ellipse\n";
    description += "    The first point determines the center of the ellipse\n";
    description += "    The second point determines the radius along the x-axis, and the third point determines the radius along the y-axis\n";
}

void EllipseShape::initialize() {
    this->radius1 = points[0].euclideanDistance(points[1]);
    this->radius2 = points[0].euclideanDistance(points[2]);
}

bool EllipseShape::isInside(const Point &point) const {
    double a = this->radius1;
    double b = this->radius2;
    double h = points[0].x;
    double k = points[0].y;
    double leftSide = b * b * (point.x - h)*(point.x - h) + a * a * (point.y - k)*(point.y - k);
    double rightSide = a * a * b * b;
    if(leftSide <= rightSide) {
        return true;
    }
    return false;
}

bool EllipseShape::isEnoughToDraw() const {
    if(this->points.size() > 3) {
        std::cerr << "Ellipse::isEnoughToDraw: points size exceed the limit (3)" << std::endl;
        return false;
    }
    return this->points.size() == 3;
}

Shape* EllipseShape::clone() const {
    EllipseShape* newEllipse = new EllipseShape();
    newEllipse->points = this->points;
    newEllipse->borderColor = this->borderColor;
    newEllipse->fillColor = this->fillColor;
    return newEllipse;
}