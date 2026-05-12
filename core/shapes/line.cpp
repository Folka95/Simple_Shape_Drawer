#include "line.h"
using namespace std;

Line::Line() : Shape(
    SHAPE_LINE,
    "Shape Type : Line\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the end points of the line"
) {
}

void Line::initialize() {

}

bool Line::isInside(const Point &point) const {
    if(this->points[0].slope(point) == point.slope(this->points[1])) {
        return true;
    }
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

void Line::takeAction(int actionID) {

}