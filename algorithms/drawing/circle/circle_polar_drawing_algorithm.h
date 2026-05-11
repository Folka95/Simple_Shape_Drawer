#pragma once
#ifndef CIRCLE_POLAR_DRAWING_ALGORITHM_H
#define CIRCLE_POLAR_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"


class Circle_Polar_DrawingAlgorithm : public DrawingAlgorithm {
public:
    Circle_Polar_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
