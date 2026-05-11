#ifndef SIMPLE_SHAPE_DRAWER_SQUARE_POINT_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_SQUARE_POINT_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"

class Square_Point_ClippingAlgorithm : public ClippingAlgorithm{
public:
    Square_Point_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape& region, ScreenWriter *sw) const;
};



#endif
