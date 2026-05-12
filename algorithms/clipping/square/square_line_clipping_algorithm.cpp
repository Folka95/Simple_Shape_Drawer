#include "square_line_clipping_algorithm.h"
#include "../../../core/shapes/line.h"

Square_Line_ClippingAlgorithm::Square_Line_ClippingAlgorithm() : ClippingAlgorithm() {}

void Square_Line_ClippingAlgorithm::runAlgorithm(Line* line, Square* square, ScreenWriter *sw) const {
    // TODO
    std::cerr << "Square_Line_ClippingAlgorithm::runAlgorithm : Still TODO" << std::endl;
}

void Square_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (inputRegion.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());
    Square* square = dynamic_cast<Square*>(inputRegion.clone());
    this->runAlgorithm(line, square, sw);
}

