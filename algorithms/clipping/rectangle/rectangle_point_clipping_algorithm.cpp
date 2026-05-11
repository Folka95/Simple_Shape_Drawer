//
// Created by frasa on 5/11/2026.
//

#include "rectangle_point_clipping_algorithm.h"

Rectangle_Point_ClippingAlgorithm::Rectangle_Point_ClippingAlgorithm():ClippingAlgorithm() {}

void Rectangle_Point_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_POLYGON){
        std::cerr << "rectanglePointClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (region.getType() != SHAPE_RECTANGLE){
        std::cerr << "rectanglePointClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }

    sw->activate();
    if (region.isInside(shape.points[0])){
        sw->setPixel(shape.points[0].x, shape.points[0].y, shape.borderColor);
    }
    sw->deactivate();
}