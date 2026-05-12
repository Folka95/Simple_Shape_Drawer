#include "square.h"
using namespace std;

Square::Square() : Shape (
    SHAPE_SQUARE,
    "Shape Type : Square\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the center of the square and a point on any side\n"
) {
}

void Square::initialize() {
    if (points.size() > 2) {
        return;
    }
    if (points.size() == 2) {
        double dx = abs(points[0].dx(points[1]));
        double dy = abs(points[0].dy(points[1]));
        sideLength = 2 * max(dx, dy);
        topLeft = Point(points[0].x - sideLength / 2, points[0].y + sideLength / 2);
        topRight = Point(points[0].x + sideLength / 2, points[0].y + sideLength / 2);
        bottomLeft = Point(points[0].x - sideLength / 2, points[0].y - sideLength / 2);
        bottomRight = Point(points[0].x + sideLength / 2, points[0].y - sideLength / 2);
    }
    this->area = sideLength * sideLength;
}

bool Square::isInside(const Point &point) const {
    if (points.size() < 2) {
        return false;
    }
    return  point.x >= topLeft.x &&
            point.x <= topRight.x &&
            point.y <= topLeft.y &&
            point.y >= bottomLeft.y;
}

bool Square::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "Square::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* Square::clone() const {
    Square* newSquare = new Square();
    newSquare->points = this->points;
    newSquare->borderColor = this->borderColor;
    newSquare->fillColor = this->fillColor;
    newSquare->initialize();
    return newSquare;
}

Point Square::getTopLeft() const {
    return topLeft;
}

Point Square::getTopRight() const {
    return topRight;
}

Point Square::getBottomLeft() const {
    return bottomLeft;
}

Point Square::getBottomRight() const {
    return bottomRight;
}

double Square::getSideLength() const {
    return sideLength;
}


std::vector< Point > Square::getSidePoints() const {
    std::vector< Point > tmp = {topLeft, topRight, bottomLeft, bottomRight};
    Point c = this->centroid(tmp);
    std::sort(tmp.begin(), tmp.end(), [&](const Point& a, const Point& b) {
        double angleA = atan2(a.y - c.y, a.x - c.x);
        double angleB = atan2(b.y - c.y, b.x - c.x);
        return angleA < angleB;
    });
    return tmp;
}
