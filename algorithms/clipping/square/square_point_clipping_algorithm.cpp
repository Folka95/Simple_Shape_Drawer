#include "square_point_clipping_algorithm.h"

Square_Point_ClippingAlgorithm::Square_Point_ClippingAlgorithm(): ClippingAlgorithm("Square_Point_ClippingAlgorithm") {

}

void Square_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, Square* square, ScreenWriter *sw) const {
    sw->activate();
    if (square->isInside(polygon->points[0])){
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    sw->deactivate();
}

void Square_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }

    if (inputShape.getSize() != 1){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon of 1 point" << std::endl;
        return;
    }

    if (inputRegion.getType() != SHAPE_SQUARE){
        std::cerr << "Square_Point_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }

    Shape* polygon = inputShape.clone();
    Square* square = dynamic_cast<Square*>(inputRegion.clone());
    this->runAlgorithm(polygon, square, sw);
}
