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
    "PolygonShape",
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
    if(size < 3) {
        return false;
    }
    const double MOE = 1e-8; // Margin Of Error
    vector< Point > sides = this->getSidePoints();
    int cnt = 0;
    for (int i = 0; i < sides.size(); i++) {
        Point a = sides[i];
        Point b = sides[(i + 1) % sides.size()];
        if(a.x == b.x) {
            if(a.x > point.x) {
                cnt++;
            }
            continue;
        }
        if(a.y == b.y) {
            if(point.y == a.y) {
                return false;
            }
            continue;
        }
        double m = a.slope(b);
        double c = a.y - m * a.x;
        Point intersection = Point((point.y - c) / m, point.y);
        if(intersection.y < min(a.y, b.y) - MOE || intersection.y > max(a.y, b.y) + MOE) {
            continue;
        }
        if(intersection.x < min(a.x, b.x) - MOE || intersection.x > max(a.x, b.x) + MOE) {
            continue;
        }
        if(intersection.x < point.x - MOE) {
            continue;
        }
        if(abs(intersection.x - point.x) < MOE) {
            return false;
        }
        cnt++;
    }
    for (int i = 0; i < sides.size(); i++) {
        Point b = sides[i];
        Point a = sides[(i + 1) % sides.size()];
        Point c = sides[(i + 2) % sides.size()];
        if(a.y != point.y) {
            continue;
        }
        if(a.x < point.x) {
            continue;
        }
        int bPos = 0, cPos = 0;
        if(b.y > a.y) {
            bPos = 1;
        }
        else if(b.y < a.y) {
            bPos = -1;
        }
        if(c.y > a.y) {
            cPos = 1;
        }
        else if(c.y < a.y) {
            cPos = -1;
        }
        if(cPos != 0 && bPos != 0 && bPos != cPos) {
            cnt--;
        }
    }
    return cnt % 2 == 1;
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
