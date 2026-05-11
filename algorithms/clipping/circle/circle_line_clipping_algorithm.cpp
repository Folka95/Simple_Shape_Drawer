#include "circle_line_clipping_algorithm.h"

Circle_Line_ClippingAlgorithm::Circle_Line_ClippingAlgorithm() : ClippingAlgorithm() {

}

void Circle_Line_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_LINE) {
        std::cerr << "lineClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if(region.getType() != SHAPE_CIRCLE) {
        std::cerr << "lineClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    // TODO
    std::cerr << "lineClippingAlgorithm::clip : Still TODO" << std::endl;
}