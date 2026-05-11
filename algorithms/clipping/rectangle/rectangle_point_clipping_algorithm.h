//
// Created by frasa on 5/11/2026.
//

#ifndef SIMPLE_SHAPE_DRAWER_RECTANGLE_POINT_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_RECTANGLE_POINT_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"

class Rectangle_Point_ClippingAlgorithm : public ClippingAlgorithm{
public:
    Rectangle_Point_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape& region, ScreenWriter *sw) const;
};



#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_POINT_CLIPPING_ALGORITHM_H
