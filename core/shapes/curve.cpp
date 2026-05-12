#include "curve.h"
using namespace std;

CurveShape::CurveShape() : Shape(
    SHAPE_CURVE,
    "Shape Type : Curve\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 4 clicks represents \n" +
    "    the start point, two control points, and the end point of the curve\n"
) {
    fixedLimit = -1;
}

void CurveShape::initialize() {

}

bool CurveShape::isInside(const Point &point) const {
    return false;
}

bool CurveShape::isEnoughToDraw() const {
    if(fixedLimit == -1) {
        return false;
    }
    if(this->points.size() > fixedLimit) {
        std::cerr << "Curve::isEnoughToDraw: points size does not match that fixed limit (" << fixedLimit << ")" << std::endl;
        return false;
    }
    return fixedLimit >= 4;
}

Shape* CurveShape::clone() const {
    Shape *tmp = new CurveShape();
    tmp->points = this->points;
    tmp->borderColor = this->borderColor;
    tmp->fillColor = this->fillColor;
    tmp->initialize();
    return tmp;
}

void CurveShape::takeAction(int actionID) {
    if(this->points.size() < 4) {
        std::cerr << "Curve::isEnoughToDraw: points size is below the limit (4)" << std::endl;
        return;
    }
    fixedLimit = this->points.size();
    this->initialize();
}

std::vector< Point > CurveShape::getSidePoints() const {
    return {};
}