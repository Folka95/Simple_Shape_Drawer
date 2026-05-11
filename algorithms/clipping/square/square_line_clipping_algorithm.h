#ifndef SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"

class Square_Line_ClippingAlgorithm : public ClippingAlgorithm {
public:
    Square_Line_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};


#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_LINE_CLIPPING_ALGORITHM_H
