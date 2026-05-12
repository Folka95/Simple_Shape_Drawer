#pragma once
#ifndef CIRCLE_LINE_CLIPPING_ALGORITHM_H
#define CIRCLE_LINE_CLIPPING_ALGORITHM_H
#include "../../clipping_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../../core/shapes/circle.h"

class Circle_Line_ClippingAlgorithm : public ClippingAlgorithm {
private:
    void drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const;
    void runAlgorithm(Line* line, Circle* circle, ScreenWriter *sw) const;
public:
    Circle_Line_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};



#endif //CIRCLE_LINE_CLIPPING_ALGORITHM_H
