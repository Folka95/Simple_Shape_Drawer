#pragma once
#ifndef LINE_MIDPOINT_DRAWING_ALGORITHM_H
#define LINE_MIDPOINT_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"


class Line_Midpoint_DrawingAlgorithm : public DrawingAlgorithm {
public:
    Line_Midpoint_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
