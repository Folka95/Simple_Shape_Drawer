#include "happy_smile_face.h"
using namespace std;

HappySmileFace::HappySmileFace() : Shape(SHAPE_HAPPY_SMILE_FACE, "HappySmileFace",
    "Shape Type : Happy Smile Face\n"s +
    "How to draw ?\n" +
    "    Use mouse left-click to make 2 clicks represents \n" +
    "    the center of the face and a point on the circumference\n"
) {

}

void HappySmileFace::initialize() {
    this->radius = points[0].euclideanDistance(points[1]);
    this->area = M_PI * this->radius * this->radius;
}

bool HappySmileFace::isInside(const Point &point) const {
    double lhs = (point.x - points[0].x) * (point.x - points[0].x) + (point.y - points[0].y) * (point.y - points[0].y);
    double rhs = radius * radius;
    return lhs <= rhs;
}

bool HappySmileFace::isEnoughToDraw() const {
    if(this->points.size() > 2) {
        std::cerr << "HappySmileFace::isEnoughToDraw: points size exceed the limit (2)" << std::endl;
        return false;
    }
    return this->points.size() == 2;
}

Shape* HappySmileFace::clone() const {
    HappySmileFace* newFace = new HappySmileFace();
    newFace->points = this->points;
    newFace->borderColor = this->borderColor;
    newFace->fillColor = this->fillColor;
    newFace->initialize();
    newFace->initialize();
    return newFace;
}

std::vector< Point > HappySmileFace::getSidePoints() const {
    std::vector< Point > res;
    if (this->radius <= 0) return res;
    double step = 1.0 / this->radius;
    for (double radian = 0; radian < 2 * M_PI; radian += step) {
        double x = (this->points[0].x + this->radius * cos(radian));
        double y = (this->points[0].y + this->radius * sin(radian));
        res.push_back(Point(x, y));
    }
    return res;
}