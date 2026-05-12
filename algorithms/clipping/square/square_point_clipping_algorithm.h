#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_SQUARE_POINT_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_SQUARE_POINT_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"
#include "../../../core/shapes/square.h"

class Square_Point_ClippingAlgorithm : public ClippingAlgorithm{
private:
    void runAlgorithm(Shape* polygon, Square* square, ScreenWriter *sw) const;
public:
    Square_Point_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape& region, ScreenWriter *sw) const;
};



#endif
