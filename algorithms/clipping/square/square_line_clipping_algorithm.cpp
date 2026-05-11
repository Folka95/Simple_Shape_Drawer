#include "square_line_clipping_algorithm.h"
#include "../../../core/shapes/line.h"

Square_Line_ClippingAlgorithm::Square_Line_ClippingAlgorithm() : ClippingAlgorithm() {}

void Square_Line_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_LINE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (region.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }
    // TODO
    std::cerr << "Square_Line_ClippingAlgorithm::clip : Still TODO" << std::endl;
}
