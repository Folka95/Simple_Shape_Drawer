#pragma once
#ifndef POLYGON_H
#define POLYGON_H
#include "../shape.h"
using namespace std;

template< int size >
class PolygonShape : public Shape {
public:
    PolygonShape();
    int getSize() const ;
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override ;
    Shape* clone() const override;
    std::vector< Point > getSidePoints() const override ;
};

template< int size >
PolygonShape<size>::PolygonShape() : Shape(
    SHAPE_POLYGON,
    "Shape Type : Polygon of "s +
    std::to_string(size) +
    " points \n" +
    "How to draw ?\n" +
    "    Use mouse left-click to make " +
    std::to_string(size) +
    " clicks represents \n" +
    "    the points of the polygon\n"
) {

}

template< int size >
void PolygonShape<size>::initialize() {
    this->area = 0;
}

template< int size >
int PolygonShape<size>::getSize() const {
    return size;
}

template< int size >
bool PolygonShape<size>::isInside(const Point &point) const {
    // TODO
    return false;
}

template< int size >
bool PolygonShape<size>::isEnoughToDraw() const {
    if(this->points.size() > size) {
        std::cerr << "PolygonShape::isEnoughToDraw: points size exceed the limit (" << size << ")" << std::endl;
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
    newPolygon->initialize();
    return newPolygon;
}

template< int size >
void PolygonShape<size>::takeAction(int actionID) {

}

template< int size >
std::vector< Point > PolygonShape<size>::getSidePoints() const {
    std::vector< Point > tmp = this->points;
    Point c = this->centroid(tmp);
    std::sort(tmp.begin(), tmp.end(), [&](const Point& a, const Point& b) {
        double angleA = atan2(a.y - c.y, a.x - c.x);
        double angleB = atan2(b.y - c.y, b.x - c.x);
        return angleA < angleB;
    });
    return tmp;
}

#endif
