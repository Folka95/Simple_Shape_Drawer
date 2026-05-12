#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_RECTANGLE_DRAWING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_RECTANGLE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../../core/shapes/rectangle.h"

class Rectangle_DrawingAlgorithm: public DrawingAlgorithm{
private:
    void drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const;
    void runAlgorithm(RectangleShape* rectangle, ScreenWriter *sw) const;
public:
    Rectangle_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};


#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_DRAWING_ALGORITHM_H
