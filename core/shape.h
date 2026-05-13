#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <vector>
#include <windows.h>
#include <cmath>
#include <iostream>
#include "algorithm"

enum ShapeType {
    SHAPE_NONE,
    SHAPE_POLYGON,
    SHAPE_LINE,
    SHAPE_CIRCLE,
    SHAPE_ELLIPSE,
    SHAPE_CURVE,
    SHAPE_RECTANGLE,
    SHAPE_SQUARE,
    SHAPE_HAPPY_SMILE_FACE,
    SHAPE_SAD_SMILE_FACE
};

class Shape {
private:
protected:
    double area;
    Shape(ShapeType type, std::string name, std::string description);
    std::string name;
    std::string description;
    ShapeType type;
    Point centroid(const std::vector< Point > &givenPoints) const;
public:
    std::vector<Point> points;
    COLORREF borderColor;
    COLORREF fillColor;
    bool addPoint(const Point &point);
    void clear();
    std::string getDescription() const ;
    std::string getName() const ;
    ShapeType getType() const ;
    int getSize() const ;
    virtual void takeAction(int actionID);
    virtual void initialize() = 0 ;
    virtual bool isInside(const Point &point) const = 0 ;
    virtual bool isEnoughToDraw() const = 0 ;
    virtual Shape* clone() const = 0;
    virtual std::vector< Point > getSidePoints() const = 0;
    double getArea() const ;
};

#endif