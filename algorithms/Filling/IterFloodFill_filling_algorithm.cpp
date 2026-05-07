#include "IterFloodFill_filling_algorithm.h"

IterFloodFill_FillingAlgorithm::IterFloodFill_FillingAlgorithm() : FillingAlgorithm() {

}

void IterFloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    sw->activate();
    std::vector<Point> stack;
    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y);
    stack.push_back(startPoint);
    while(!stack.empty()) {
        Point current = stack.back();
        stack.pop_back();

        if(current.x < 0 || current.x >= sw->getWidth() || current.y < 0 || current.y >= sw->getHeight()) {
            continue;
        }

        COLORREF currentColor = sw->getPixel(current.x, current.y);
        if( shape.isInside(current) && currentColor == fillableColor &&
            currentColor != shape.borderColor && currentColor != shape.fillColor) {
            sw->setPixel(current.x, current.y, shape.fillColor);
            stack.push_back(Point(current.x + 1, current.y));
            stack.push_back(Point(current.x - 1, current.y));
            stack.push_back(Point(current.x, current.y + 1));
            stack.push_back(Point(current.x, current.y - 1));
        }
    }
    sw->deactivate();
}