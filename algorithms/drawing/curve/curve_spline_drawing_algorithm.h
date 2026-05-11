#pragma once
#ifndef CURVE_SPLINE_DRAWING_ALGORITHM_H
#define CURVE_SPLINE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"


class Curve_Spline_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void drawCurveHermite(Point t0, Point t1, Point p0, Point p1, ScreenWriter *sw, const Shape &shape) const;
public:
    Curve_Spline_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
