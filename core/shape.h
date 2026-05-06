#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <vector>
#include <windows.h>

enum ShapeType {
    SHAPE_NONE,
    SHAPE_LINE,
    SHAPE_CIRCLE,
    SHAPE_ELLIPSE,
    SHAPE_CURVE,
    SHAPE_RECTANGLE,
    SHAPE_SQUARE
};

class Shape {
private:
protected:
    Shape();
    std::string description;
    ShapeType type;
public:
    std::vector<Point> points;
    COLORREF borderColor;
    COLORREF fillColor;
    bool addPoint(const Point &point);
    void clear();
    std::string getDescription() const ;
    ShapeType getType() const ;
    virtual bool isInside(const Point &point) const = 0 ;
    virtual bool isEnoughToDraw() const = 0 ;
    virtual Shape* clone() const = 0;
};

#endif