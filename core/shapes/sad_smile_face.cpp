#include "sad_smile_face.h"
using namespace std;

SadSmileFace::SadSmileFace() : Shape (
    SHAPE_SAD_SMILE_FACE,
    "Shape Type : Sad Smile Face\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the center of the face and a point on the circumference\n"
) {

}

void SadSmileFace::initialize() {
    this->radius = points[0].euclideanDistance(points[1]);
    this->area = M_PI * this->radius * this->radius;
}

bool SadSmileFace::isInside(const Point &point) const {
    double lhs = (point.x - points[0].x) * (point.x - points[0].x) + (point.y - points[0].y) * (point.y - points[0].y);
    double rhs = radius * radius;
    return lhs <= rhs;
}

bool SadSmileFace::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "SadSmileFace::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* SadSmileFace::clone() const {
    SadSmileFace* newFace = new SadSmileFace();
    newFace->points = this->points;
    newFace->borderColor = this->borderColor;
    newFace->fillColor = this->fillColor;
    newFace->initialize();
    return newFace;
}

std::vector< Point > SadSmileFace::getSidePoints() const {
    return {};
}