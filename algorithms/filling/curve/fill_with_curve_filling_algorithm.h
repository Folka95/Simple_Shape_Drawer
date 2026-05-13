#pragma once
#ifndef FILL_WITH_CURVE_FILLING_ALGORITHM_H
#define FILL_WITH_CURVE_FILLING_ALGORITHM_H

#include "../../filling_algorithm.h"
#include "../../../core/shapes/square.h"
#include "../../../core/shapes/rectangle.h"

class FillWithCurveFillingAlgorithm : public FillingAlgorithm {
private:
    void fill_horizontal(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
    void fill_vertical(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
    void drawCurveHermite(Shape* shape,Point t0, Point t1, Point p0, Point p3, ScreenWriter *sw) const;
    void drawCurveBezier(Shape* shape,Point c1, Point c2, Point p0, Point p3, ScreenWriter *sw) const;
public:
    FillWithCurveFillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};

#endif