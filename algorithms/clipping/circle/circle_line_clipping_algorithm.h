#ifndef CIRCLE_LINE_CLIPPING_ALGORITHM_H
#define CIRCLE_LINE_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"

class Circle_line_ClippingAlgorithm : public ClippingAlgorithm {
public:
    Circle_line_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};



#endif //CIRCLE_LINE_CLIPPING_ALGORITHM_H
