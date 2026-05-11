#include "curve.h"

CurveShape::CurveShape() : Shape() {
    type = SHAPE_CURVE;
    description = "";
    description += "Shape Type : Curve\n";
    description += "How to draw ?\n";
    description += "    Use mouse left-click to make 4 clicks represents \n";
    description += "    the start point, two control points, and the end point of the curve\n";
    fixedLimit = -1;
}

void CurveShape::initialize() {

}

bool CurveShape::isInside(const Point &point) const {
    return false;
}

bool CurveShape::isEnoughToDraw() const {
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
    return tmp;
}

void CurveShape::takeAction(int actionID) {
    if(this->points.size() < 4) {
        std::cerr << "Curve::isEnoughToDraw: points size is below the limit (4)" << std::endl;
        return;
    }
    fixedLimit = this->points.size();
}