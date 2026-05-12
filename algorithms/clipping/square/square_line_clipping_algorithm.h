#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"
#include "../../../core/shapes/square.h"
#include "../../../core/shapes/line.h"

class Square_Line_ClippingAlgorithm : public ClippingAlgorithm {
private:
    union OutCode;
    OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) const;
    void drawLine(const Point &p1, const Point &p2, COLORREF borderColor, ScreenWriter *sw) const;
    void runAlgorithm(Line* line, Square* square, ScreenWriter *sw) const;
    void VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const;
    void HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const;
public:
    Square_Line_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};


#endif //SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H
