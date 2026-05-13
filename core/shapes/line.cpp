#include "line.h"
using namespace std;

Line::Line() : Shape(SHAPE_LINE, "Line",
    "Shape Type : Line\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the end points of the line"
) {
}

void Line::initialize() {

}

bool Line::isInside(const Point &point) const {
    return false;
}


bool Line::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "Line::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* Line::clone() const {
    Shape *tmp = new Line();
    tmp->points = this->points;
    tmp->borderColor = this->borderColor;
    tmp->fillColor = this->fillColor;
    tmp->initialize();
    return tmp;
}

std::vector< Point > Line::getSidePoints() const {
    std::vector< Point > tmp = this->points;
    Point c = this->centroid(tmp);
    std::sort(tmp.begin(), tmp.end(), [&](const Point& a, const Point& b) {
        double angleA = atan2(a.y - c.y, a.x - c.x);
        double angleB = atan2(b.y - c.y, b.x - c.x);
        return angleA < angleB;
    });
    return tmp;
}