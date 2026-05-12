#include "circle_quarter_with_line_filling_algorithm.h"

Circle_QuarterWithLine_FillingAlgorithm::Circle_QuarterWithLine_FillingAlgorithm() : FillingAlgorithm() {

}


void Circle_QuarterWithLine_FillingAlgorithm::drawMidpointLine(int x1, int y1, int x2, int y2, ScreenWriter *sw, COLORREF color) const {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int x = x1;
    int y = y1;

    sw->setPixel(x, y, color);

    if (dx >= dy) {
        int d = 2 * dy - dx;
        while (x != x2) {
            if (d >= 0) {
                y += sy;
                d += 2 * (dy - dx);
            } else {
                d += 2 * dy;
            }
            x += sx;
            sw->setPixel(x, y, color);
        }
    } else {
        int d = 2 * dx - dy;
        while (y != y2) {
            if (d >= 0) {
                x += sx;
                d += 2 * (dx - dy);
            } else {
                d += 2 * dx;
            }
            y += sy;
            sw->setPixel(x, y, color);
        }
    }
}

void Circle_QuarterWithLine_FillingAlgorithm::fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const{
    int q = circle.scanQuarter(startPoint);
    Point center = circle.getCenter();
    double radius = circle.getRadius();

    if (q == 0 || q == 5) {
        std::cerr << "Circle_QuarterWithLine_FillingAlgorithm::fill_helper : Please Choose a valid quarter" << std::endl;
        return;
    }

    int x = 0;
    int y = (int)radius;
    int d = 1 - (int)radius;

    while (x <= y) {
        if (q == 1) { // Top-Right
            drawMidpointLine(center.x, center.y + y, center.x + x, center.y + y, sw, circle.fillColor);
            drawMidpointLine(center.x, center.y + x, center.x + y, center.y + x, sw, circle.fillColor);
        } else if (q == 2) { // Top-Left
            drawMidpointLine(center.x, center.y + y, center.x - x, center.y + y, sw, circle.fillColor);
            drawMidpointLine(center.x, center.y + x, center.x - y, center.y + x, sw, circle.fillColor);
        } else if (q == 3) { // Bottom-Left
            drawMidpointLine(center.x, center.y - y, center.x - x, center.y - y, sw, circle.fillColor);
            drawMidpointLine(center.x, center.y - x, center.x - y, center.y - x, sw, circle.fillColor);
        } else if (q == 4) { // Bottom-Right
            drawMidpointLine(center.x, center.y - y, center.x + x, center.y - y, sw, circle.fillColor);
            drawMidpointLine(center.x, center.y - x, center.x + y, center.y - x, sw, circle.fillColor);
        }

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void Circle_QuarterWithLine_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_CIRCLE){
        std::cerr << "Circle_QuarterWithLine_FillingAlgorithm::fill : Region must be a Circle" << std::endl;
        return;
    }
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (!circle) return;

    sw->activate();
    this->fill_helper(*circle, clippingRegion, startPoint, sw);
    sw->deactivate();
}