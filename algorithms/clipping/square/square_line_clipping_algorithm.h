#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"
#include "../../../core/shapes/square.h"
#include "../../../core/shapes/line.h"

class Square_Line_ClippingAlgorithm : public ClippingAlgorithm {
private:
    void runAlgorithm(Line* line, Square* square, ScreenWriter *sw) const;
public:
    Square_Line_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};


#endif //SIMPLE_SHAPE_DRAWER_SQUARE_LINE_CLIPPING_ALGORITHM_H
