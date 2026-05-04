#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include "shape.h"
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

enum ShapeType {
    SHAPE_LINE,
    SHAPE_CIRCLE
};

class Shape {
private:
    Shape() {}
protected:
    string description;
    ShapeType type;
public:
    vector< Point< int > > points;
    Shape(const Shape &other);
    COLORREF borderColor;
    COLORREF fillColor;
    bool addPoint(const Point< int > &point);
    bool isEnoughToDraw();
    void clear();
    string getDescription();
};

#endif //SHAPE_H
