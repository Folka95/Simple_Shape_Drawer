#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include "shape.h"
#include <bits/stdc++.h>
using namespace std;

class Shape {
private:
    string description;
    Shape() {}
protected:
    string setDescription();
public:
    Shape(const Shape &other);
    vector< Point< int > > points;
    void addPoint(const Point< int > &point);
    void clear();
};

#endif //SHAPE_H
