#include "shape.h"

Shape::Shape(ShapeType type, std::string description) {
    this->points.clear();
    this->borderColor = RGB(255, 255, 255);
    this->fillColor = RGB(255, 255, 255);
    this->type = type;
    this->description = description;
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