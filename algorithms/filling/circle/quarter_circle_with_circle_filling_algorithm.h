#pragma once
#ifndef QUARTERCIRCLE_WITH_CIRCLE_FILLING_ALGORITHM_H
#define QUARTERCIRCLE_WITH_CIRCLE_FILLING_ALGORITHM_H

#include "../../filling_algorithm.h"
#include "../../../core/shapes/circle.h"

class QuarterCircleWithCircle_FillingAlgorithm : public FillingAlgorithm {
private:
    void fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
    void drawQuarterCircle(double xc, double yc, double radius, int q, ScreenWriter *sw, COLORREF color) const;
public:
    QuarterCircleWithCircle_FillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};

#endif