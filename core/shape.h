#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include "shape.h"
#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

class Shape {
private:
    Shape() {}
protected:
    string description;
public:
    Shape(const Shape &other);
    COLORREF borderColor;
    COLORREF fillColor;
    vector< Point< int > > points;
    bool addPoint(const Point< int > &point);
    bool isEnoughToDraw();
    void clear();
    string getDescription();
};

#endif //SHAPE_H
