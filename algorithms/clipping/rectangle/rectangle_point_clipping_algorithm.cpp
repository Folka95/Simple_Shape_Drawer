//
// Created by frasa on 5/11/2026.
//

#include "rectangle_point_clipping_algorithm.h"

Rectangle_Point_ClippingAlgorithm::Rectangle_Point_ClippingAlgorithm():ClippingAlgorithm() {}

void Rectangle_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, RectangleShape* rectangle, ScreenWriter *sw) const {
    sw->activate();
    if (rectangle->isInside(polygon->points[0])){
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    sw->deactivate();
}

void Rectangle_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON){
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputShape.getSize() != 1){
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon of 1 point" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_RECTANGLE){
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }
    Shape* polygon = inputShape.clone();
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputRegion.clone());
    this->runAlgorithm(polygon, rectangle, sw);
}