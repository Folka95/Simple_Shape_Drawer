#include "circle_line_clipping_algorithm.h"

Circle_Line_ClippingAlgorithm::Circle_Line_ClippingAlgorithm() : ClippingAlgorithm() {

}

void Circle_Line_ClippingAlgorithm::runAlgorithm(Line* line, Circle* circle, ScreenWriter *sw) const {
    // TODO
    std::cerr << "Circle_Line_ClippingAlgorithm::runAlgorithm : Still TODO" << std::endl;
}

void Circle_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if(inputRegion.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());
    Circle* circle = dynamic_cast<Circle*>(inputRegion.clone());
    this->runAlgorithm(line, circle, sw);
}