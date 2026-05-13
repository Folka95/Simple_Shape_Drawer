#include "iterive_flood_fill_filling_algorithm.h"

IterFloodFill_FillingAlgorithm::IterFloodFill_FillingAlgorithm() : FillingAlgorithm("IterFloodFill_FillingAlgorithm") {

}

void IterFloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (!clippingRegion.isInside(startPoint)) {
        return;
    }
    sw->activate();
    std::stack<Point> stack;
    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y);
    stack.push(startPoint);
    while(!stack.empty()) {
        Point current = stack.top();
        stack.pop();

        if(sw->outOfBounds(current.x, current.y)) {
            continue;
        }

        COLORREF currentColor = sw->getPixel(current.x, current.y);

        if(shape.isInside(current) && currentColor == fillableColor && currentColor != shape.borderColor && currentColor != shape.fillColor && (&clippingRegion == nullptr || clippingRegion.isInside(current))) {
            sw->setPixel(current.x, current.y, shape.fillColor);
            Point right(current.x + 1, current.y);
            if (!sw->outOfBounds(right.x, right.y) && shape.isInside(right)) {
                stack.push(right);
            }
            Point left(current.x - 1, current.y);
            if (!sw->outOfBounds(left.x, left.y) && shape.isInside(left)) {
                stack.push(left);
            }
            Point up(current.x, current.y + 1);
            if (!sw->outOfBounds(up.x, up.y) && shape.isInside(up)) {
                stack.push(up);
            }
            Point down(current.x, current.y - 1);
            if (!sw->outOfBounds(down.x, down.y) && shape.isInside(down)) {
                stack.push(down);
            }
        }
    }
    sw->deactivate();
}