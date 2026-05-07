#include "circle_point_clipping_algorithm.h"

Circle_Point_ClippingAlgorithm::Circle_Point_ClippingAlgorithm() : ClippingAlgorithm() {

}

void Circle_Point_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_POLYGON) {
        std::cerr << "lineClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if(region.getType() != SHAPE_CIRCLE) {
        std::cerr << "lineClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    sw->activate();
    if(region.isInside(shape.points[0])) {
        sw->setPixel(shape.points[0].x, shape.points[0].y, shape.borderColor);
    }
    sw->deactivate();
}