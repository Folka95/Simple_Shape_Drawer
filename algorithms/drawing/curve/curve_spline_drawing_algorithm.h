#pragma once
#ifndef CURVE_SPLINE_DRAWING_ALGORITHM_H
#define CURVE_SPLINE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/curve.h"


class Curve_Spline_DrawingAlgorithm : public DrawingAlgorithm {
private:
    double Cvalue;
    void drawCurveHermite(Point t0, Point t1, Point p0, Point p1, ScreenWriter *sw, CurveShape* curve) const;
    void runAlgorithm(CurveShape* curve, ScreenWriter *sw) const ;
public:
    Curve_Spline_DrawingAlgorithm(int Cvalue);
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
