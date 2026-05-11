#pragma once
#ifndef POLYGON_H
#define POLYGON_H
#include "../shape.h"

template< int size >
class PolygonShape : public Shape {
public:
    PolygonShape();
    int getSize() const ;
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
};

template< int size >
PolygonShape<size>::PolygonShape() : Shape() {
    type = SHAPE_POLYGON;
    description = "";
    description += "Shape Type : Polygon of ";
    description += std::to_string(size);
    description += " points \n";
    description += "How to draw ?\n";
    description += "    Use mouse left-click to make";
    description += std::to_string(size);
    description += " clicks represents \n";
    description += "    the points of the polygon\n";
}

template< int size >
void PolygonShape<size>::initialize() {

}

template< int size >
int PolygonShape<size>::getSize() const {
    return size;
}

template< int size >
bool PolygonShape<size>::isInside(const Point &point) const {
    // TODO
    return true;
}

template< int size >
bool PolygonShape<size>::isEnoughToDraw() const {
    if(this->points.size() > size) {
        std::cerr << "Circle::isEnoughToDraw: points size exceed the limit (" << size << ")" << std::endl;
        return false;
    }
    return this->points.size() == size;
}

template< int size >
Shape* PolygonShape<size>::clone() const {
    PolygonShape* newPolygon = new PolygonShape();
    newPolygon->points = this->points;
    newPolygon->borderColor = this->borderColor;
    newPolygon->fillColor = this->fillColor;
    return newPolygon;
}

template< int size >
void PolygonShape<size>::takeAction(int actionID) {

}

#endif
