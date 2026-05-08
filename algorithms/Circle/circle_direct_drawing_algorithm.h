#pragma once
#ifndef CIRCLE_DIRECT_DRAWING_ALGORITHM_H
#define CIRCLE_DIRECT_DRAWING_ALGORITHM_H
#include "../drawing_algorithm.h"


class Circle_Direct_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const;
public:
    Circle_Direct_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
