#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"
#include "../../../core/shapes/rectangle.h"

class Rectangle_Polygon_ClippingAlgorithm : public ClippingAlgorithm{
private:
    void runAlgorithm(Shape* polygon, RectangleShape* rectangle, ScreenWriter *sw) const;
public:
    Rectangle_Polygon_ClippingAlgorithm();
    void clip(const Shape &shape, const Shape& region, ScreenWriter *sw) const;
};


#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H
