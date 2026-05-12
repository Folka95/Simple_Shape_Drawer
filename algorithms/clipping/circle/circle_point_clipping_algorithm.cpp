#include "circle_point_clipping_algorithm.h"

Circle_Point_ClippingAlgorithm::Circle_Point_ClippingAlgorithm() : ClippingAlgorithm() {

}

void Circle_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, Circle* circle, ScreenWriter *sw) const {
    sw->activate();
    if(circle->isInside(polygon->points[0])) {
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    sw->deactivate();
}

void Circle_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Circle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if(inputRegion.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Point_ClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    Shape* polygon = inputShape.clone();
    Circle* circle = dynamic_cast<Circle*>(inputRegion.clone());
    this->runAlgorithm(polygon, circle, sw);
}