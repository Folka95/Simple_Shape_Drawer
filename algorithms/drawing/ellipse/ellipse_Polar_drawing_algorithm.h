#pragma once
#ifndef ELLIPSE_POLAR_DRAWING_ALGORITHM_H
#define ELLIPSE_POLAR_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/ellipse.h"


class Ellipse_Polar_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const;
public:
    Ellipse_Polar_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
