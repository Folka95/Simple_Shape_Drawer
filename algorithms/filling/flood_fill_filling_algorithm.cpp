#include "flood_fill_filling_algorithm.h"

FloodFill_FillingAlgorithm::FloodFill_FillingAlgorithm() : FillingAlgorithm() {

}

void FloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    sw->activate();
    fill_helper(shape, clippingRegion, startPoint, sw);
    sw->deactivate();
}

void FloodFill_FillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y);
    if( shape.isInside(startPoint) && !sw->outOfBounds(startPoint.x, startPoint.y) && fillableColor != shape.borderColor && fillableColor != shape.fillColor) {
        sw->setPixel(startPoint.x, startPoint.y, shape.fillColor);
        Point right(startPoint.x + 1, startPoint.y);
        if (!sw->outOfBounds(right.x, right.y)) {
            fill_helper(shape, clippingRegion, right, sw);
        }
        Point left(startPoint.x - 1, startPoint.y);
        if (!sw->outOfBounds(left.x, left.y)) {
            fill_helper(shape, clippingRegion, left, sw);
        }
        Point down(startPoint.x, startPoint.y + 1);
        if (!sw->outOfBounds(down.x, down.y)) {
            fill_helper(shape, clippingRegion, down, sw);
        }
        Point up(startPoint.x, startPoint.y - 1);
        if (!sw->outOfBounds(up.x, up.y)) {
            fill_helper(shape, clippingRegion, up, sw);
        }
    }
}