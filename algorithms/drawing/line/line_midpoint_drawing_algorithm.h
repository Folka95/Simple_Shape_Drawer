#pragma once
#ifndef LINE_MIDPOINT_DRAWING_ALGORITHM_H
#define LINE_MIDPOINT_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/line.h"


class Line_Midpoint_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void runAlgorithm(Line* line, ScreenWriter *sw) const;
public:
    Line_Midpoint_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
