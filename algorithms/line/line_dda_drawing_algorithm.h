#pragma once
#ifndef LINE_DDA_DRAWING_ALGORITHM_H
#define LINE_DDA_DRAWING_ALGORITHM_H
#include "../drawing_algorithm.h"


class Line_DDA_DrawingAlgorithm : public DrawingAlgorithm {
public:
    Line_DDA_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
