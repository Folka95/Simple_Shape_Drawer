#include "flood_fill_filling_algorithm.h"

FloodFill_FillingAlgorithm::FloodFill_FillingAlgorithm() : FillingAlgorithm("FloodFill_FillingAlgorithm") {

}

void FloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (!clippingRegion.isInside(startPoint)) {
        return;
    }
    sw->activate();
    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y);
    fill_helper(shape, clippingRegion, startPoint, fillableColor,sw);
    sw->deactivate();
}

void FloodFill_FillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, COLORREF fillableColor , ScreenWriter *sw) const {
    COLORREF currentColor = sw->getPixel(startPoint.x, startPoint.y);
    if( shape.isInside(startPoint) && !sw->outOfBounds(startPoint.x, startPoint.y) && currentColor == fillableColor && currentColor != shape.fillColor && (&clippingRegion == nullptr || clippingRegion.isInside(startPoint))) {
        sw->setPixel(startPoint.x, startPoint.y, shape.fillColor);
        Point right(startPoint.x + 1, startPoint.y);
        if (!sw->outOfBounds(right.x, right.y) && shape.isInside(right)) {
            fill_helper(shape, clippingRegion, right, fillableColor ,sw);
        }
        Point left(startPoint.x - 1, startPoint.y);
        if (!sw->outOfBounds(left.x, left.y) && shape.isInside(left)) {
            fill_helper(shape, clippingRegion, left, fillableColor ,sw);
        }
        Point down(startPoint.x, startPoint.y + 1);
        if (!sw->outOfBounds(down.x, down.y) && shape.isInside(down)) {
            fill_helper(shape, clippingRegion, down, fillableColor ,sw);
        }
        Point up(startPoint.x, startPoint.y - 1);
        if (!sw->outOfBounds(up.x, up.y) && shape.isInside(up)) {
            fill_helper(shape, clippingRegion, up, fillableColor ,sw);
        }
    }
}