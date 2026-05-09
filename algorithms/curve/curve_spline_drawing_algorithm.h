#pragma once
#ifndef CURVE_SPLINE_DRAWING_ALGORITHM_H
#define CURVE_SPLINE_DRAWING_ALGORITHM_H
#include "../drawing_algorithm.h"


class Curve_Spline_DrawingAlgorithm : public DrawingAlgorithm {
public:
    Curve_Spline_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
