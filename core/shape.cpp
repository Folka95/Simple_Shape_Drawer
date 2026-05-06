#include "shape.h"

Shape::Shape() {
    this->points.clear();
    this->borderColor = RGB(255, 255, 255);
    this->fillColor = RGB(255, 255, 255);
}

bool Shape::addPoint(const Point &point) {
    this->points.push_back(point);
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
