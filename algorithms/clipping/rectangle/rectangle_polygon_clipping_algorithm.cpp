#include "rectangle_polygon_clipping_algorithm.h"

Rectangle_Polygon_ClippingAlgorithm::Rectangle_Polygon_ClippingAlgorithm(): ClippingAlgorithm() {}

void Rectangle_Polygon_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_POLYGON){
        std::cerr << "rectanglePointClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (region.getType() != SHAPE_RECTANGLE){
        std::cerr << "rectanglePointClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }

    // TODO
    std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : Still TODO" << std::endl;
}