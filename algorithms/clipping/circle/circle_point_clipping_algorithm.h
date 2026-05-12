#pragma once
#ifndef CIRCLE_POINT_CLIPPING_ALGORITHM_H
#define CIRCLE_POINT_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"
#include "../../../core/shapes/circle.h"


class Circle_Point_ClippingAlgorithm : public ClippingAlgorithm {
private:
    void runAlgorithm(Shape* polygon, Circle* circle, ScreenWriter *sw) const;
public:
    Circle_Point_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};



#endif
