#pragma once
#ifndef QUARTERCIRCLE_WITHLINE_FILLING_ALGORITHM_H
#define QUARTERCIRCLE_WITHLINE_FILLING_ALGORITHM_H
#include "../../filling_algorithm.h"
#include "../../../core/shapes/circle.h"


class Circle_QuarterWithLine_FillingAlgorithm : public FillingAlgorithm {
private:
    void fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
    void drawMidpointLine(int x1, int y1, int x2, int y2, ScreenWriter *sw, COLORREF color, const Shape &clippingRegion) const;
public:
    Circle_QuarterWithLine_FillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};



#endif
