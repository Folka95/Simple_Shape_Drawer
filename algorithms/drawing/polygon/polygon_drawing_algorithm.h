#pragma once
#ifndef POLYGON_DRAWING_ALGORITHM_H
#define POLYGON_DRAWING_ALGORITHM_H

#include "../../drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"


class Polygon_DrawingAlgorithm : public DrawingAlgorithm  {
private:
    void drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const;
public:
    Polygon_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
