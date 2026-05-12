#include "iterive_flood_fill_filling_algorithm.h"

IterFloodFill_FillingAlgorithm::IterFloodFill_FillingAlgorithm() : FillingAlgorithm() {

}

void IterFloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    sw->activate();
    std::stack<Point> stack;
    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y);
    stack.push(startPoint);
    while(!stack.empty()) {
        Point current = stack.top();
        stack.pop();

        if(current.x < 0 || current.x >= sw->getWidth() || current.y < 0 || current.y >= sw->getHeight()) {
            continue;
        }

        COLORREF currentColor = sw->getPixel(current.x, current.y);
        if( shape.isInside(current) && currentColor == fillableColor &&
            currentColor != shape.borderColor && currentColor != shape.fillColor) {
            sw->setPixel(current.x, current.y, shape.fillColor);
            stack.push(Point(current.x + 1, current.y));
            stack.push(Point(current.x - 1, current.y));
            stack.push(Point(current.x, current.y + 1));
            stack.push(Point(current.x, current.y - 1));
        }
    }
    sw->deactivate();
}