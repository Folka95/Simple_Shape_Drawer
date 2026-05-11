#include "line.h"

Line::Line() : Shape() {
    type = SHAPE_LINE;
    description = "";
    description += "Shape Type : Line\n";
    description += "How to draw ?\n";
    description += "    Use mouse left-click to make 2 clicks represents \n";
    description += "    the end points of the line";
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
    return tmp;
}

void Line::takeAction(int actionID) {

}