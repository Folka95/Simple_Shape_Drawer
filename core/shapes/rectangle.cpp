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
        topLeft = Point(min(points[0].x, points[1].x), min(points[0].y, points[1].y));
        bottomRight = Point(max(points[0].x, points[1].x), max(points[0].y, points[1].y));
        topRight = Point(bottomRight.x, topLeft.y);
        bottomLeft = Point(topLeft.x, bottomRight.y);
        width = points[1].x - points[0].x;
        height = points[1].y - points[0].y;
    }
    this->area = width * height;
}

bool RectangleShape::isInside(const Point &point) const {
    if (points.size() < 2) {
        return false;
    }
    return  point.x >= topLeft.x && point.x <= bottomRight.x &&
            point.y >= topLeft.y && point.y <= bottomRight.y;
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
    newRectangle->initialize();
    return newRectangle;
}

Point RectangleShape::getTopLeft() const {
    return topLeft;
}

Point RectangleShape::getTopRight() const {
    return topRight;
}

Point RectangleShape::getBottomLeft() const {
    return bottomLeft;
}

Point RectangleShape::getBottomRight() const {
    return bottomRight;
}

double RectangleShape::getWidth() const {
    return width;
}

double RectangleShape::getHeight() const {
    return height;
}

std::vector< Point > RectangleShape::getSidePoints() const {
    std::vector< Point > tmp = {topLeft, topRight, bottomLeft, bottomRight};
    Point c = this->centroid(tmp);
    std::sort(tmp.begin(), tmp.end(), [&](const Point& a, const Point& b) {
        double angleA = atan2(a.y - c.y, a.x - c.x);
        double angleB = atan2(b.y - c.y, b.x - c.x);
        return angleA < angleB;
    });
    return tmp;
}