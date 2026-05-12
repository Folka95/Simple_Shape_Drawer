#pragma once
#ifndef ELLIPSE_MIDPOINT_DRAWING_ALGORITHM_H
#define ELLIPSE_MIDPOINT_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/ellipse.h"


class Ellipse_MidPoint_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void Draw4Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const;
    void runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const;
public:
    Ellipse_MidPoint_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
