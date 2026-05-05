#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <vector>
#include <windows.h>

enum ShapeType {
    SHAPE_LINE,
    SHAPE_CIRCLE
};

class Shape {
private:
    Shape() {}
protected:
    std::string description;
    ShapeType type;
public:
    std::vector<Point> points;
    COLORREF borderColor;
    COLORREF fillColor;

    bool addPoint(const Point &point);
    virtual bool isEnoughToDraw() = 0;
    void clear();
    std::string getDescription();
    virtual Shape* clone() = 0;
};

#endif