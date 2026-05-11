#include "square.h"
using namespace std;

Square::Square() : Shape (
    SHAPE_SQUARE,
    "Shape Type : Square\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the center of the square and a corner point\n"
    // TODO
) {
}

void Square::initialize() {
    if (points.size() > 2) {
        return;
    }
    if (points.size() == 2) {
        // TODO
        sideLength = 0;
    }
}

bool Square::isInside(const Point &point) const {
    if (points.size() < 2) {
        return false;
    }
    // TODO
    return false;
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
    return newSquare;
}

void Square::takeAction(int actionID) {

}