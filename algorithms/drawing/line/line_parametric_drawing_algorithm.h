#pragma once
#ifndef LINE_PARAMETRIC_DRAWING_ALGORITHM_H
#define LINE_PARAMETRIC_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/line.h"


class Line_Parametric_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void runAlgorithm(Line* line, ScreenWriter *sw) const;
public:
    Line_Parametric_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
