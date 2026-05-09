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
    // 1. Calculate absolute horizontal and vertical distances from both points relative to the center
    double dx1 = abs(points[1].x - points[0].x);
    double dy1 = abs(points[1].y - points[0].y);
    double dx2 = abs(points[2].x - points[0].x);
    double dy2 = abs(points[2].y - points[0].y);

    // 2. Protect the order: 
    // radius1 is the "most horizontal" distance found in either point
    // radius2 is the "most vertical" distance found in either point
    this->radius1 = (dx1 > dx2) ? dx1 : dx2;
    this->radius2 = (dy1 > dy2) ? dy1 : dy2;

    // 3. Cleanup/Align points for internal storage logic
    // Point 1: Must stay on the center's Y-level (Horizontal Axis)
    points[1].y = points[0].y;
    // (Optional) We set its X to the center + radius1 to keep it visually "snapped"
    points[1].x = points[0].x + this->radius1; 

    // Point 2: Must stay on the center's X-level (Vertical Axis)
    points[2].x = points[0].x;
    // (Optional) We set its Y to the center + radius2 to keep it visually "snapped"
    points[2].y = points[0].y + this->radius2;
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