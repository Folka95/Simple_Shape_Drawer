#pragma once
#ifndef CIRCLE_POLAR_DRAWING_ALGORITHM_H
#define CIRCLE_POLAR_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/circle.h"


class Circle_Polar_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void runAlgorithm(Circle* circle, ScreenWriter *sw) const;
public:
    Circle_Polar_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
