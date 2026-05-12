#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_RECTANGLE_LINE_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_RECTANGLE_LINE_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"
#include "../../../core/shapes/rectangle.h"
#include "../../../core/shapes/line.h"

class Rectangle_Line_ClippingAlgorithm : public ClippingAlgorithm {
private:
    void drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const;
    void VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const;
    void HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const;
    void runAlgorithm(Line* line, RectangleShape* rectangle, ScreenWriter *sw) const;

public:
        Rectangle_Line_ClippingAlgorithm();
        void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
    };


#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_LINE_CLIPPING_ALGORITHM_H
