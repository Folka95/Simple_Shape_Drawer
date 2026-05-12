#include "rectangle_polygon_clipping_algorithm.h"

Rectangle_Polygon_ClippingAlgorithm::Rectangle_Polygon_ClippingAlgorithm(): ClippingAlgorithm() {}

void Rectangle_Polygon_ClippingAlgorithm::runAlgorithm(Shape* polygon, RectangleShape* rectangle, ScreenWriter *sw) const {
    // TODO
    std::cerr << "Rectangle_Polygon_ClippingAlgorithm::runAlgorithm : Still TODO" << std::endl;
}

void Rectangle_Polygon_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON){
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }

    if (inputRegion.getType() != SHAPE_RECTANGLE){
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }

    Shape* polygon = inputShape.clone();
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputRegion.clone());
    this->runAlgorithm(polygon, rectangle, sw);
}