#include "square_point_clipping_algorithm.h"

Square_Point_ClippingAlgorithm::Square_Point_ClippingAlgorithm(): ClippingAlgorithm() {

}

void Square_Point_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_POLYGON){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }

    if (shape.getSize() != 1){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon of 1 point" << std::endl;
        return;
    }

    if (region.getType() != SHAPE_SQUARE){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }

    sw->activate();
    if (region.isInside(shape.points[0])){
        sw->setPixel(shape.points[0].x, shape.points[0].y, shape.borderColor);
    }
    sw->deactivate();
}