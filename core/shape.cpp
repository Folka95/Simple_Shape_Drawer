#include "shape.h"
#include "algorithm"

Shape::Shape(ShapeType type, std::string description) {
    this->points.clear();
    this->borderColor = RGB(255, 255, 255);
    this->fillColor = RGB(255, 255, 255);
    this->type = type;
    this->description = description;
    this->area = 0;
}

bool Shape::addPoint(const Point &point) {
    this->points.push_back(point);
    if(this->isEnoughToDraw()) {
        this->initialize();
    }
    return this->isEnoughToDraw();
}

ShapeType Shape::getType() const {
    return this->type;
}

void Shape::clear() {
    this->points.clear();
    this->borderColor = RGB(255, 255, 255);
    this->fillColor = RGB(255, 255, 255);
}

std::string Shape::getDescription() const {
    return this->description;
}

int Shape::getSize() const {
    return this->points.size();
}

void Shape::takeAction(int actionID) {

}

Point Shape::centroid(const std::vector< Point > &givenPoints) const {
    double cx = 0, cy = 0;
    for (auto &[x, y] : givenPoints) {
        cx += x;
        cy += y;
    }
    cx /= getSize();
    cy /= getSize();
    return Point(cx, cy);
}


double Shape::getArea() const {
    return this->area;
}