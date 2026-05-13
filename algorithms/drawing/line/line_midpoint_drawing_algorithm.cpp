#include "line_midpoint_drawing_algorithm.h"
#include <bits/stdc++.h>

Line_Midpoint_DrawingAlgorithm::Line_Midpoint_DrawingAlgorithm() : DrawingAlgorithm("Line_Midpoint_DrawingAlgorithm") {

}

void Line_Midpoint_DrawingAlgorithm::runAlgorithm(Line* line, ScreenWriter *sw) const {
    Point p0(line->points[0]);
    Point p1(line->points[1]);

    int x0 = p0.x;
    int y0 = p0.y;

    int x1 = p1.x;
    int y1 = p1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    // Determine the direction of the line
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int x = x0;
    int y = y0;

    sw->activate();
    sw->setPixel(x, y, line->borderColor);
    // if the line is more horizontal than vertical
    if(dx >= dy) {
        int d = 2 * dy - dx;
        while(x != x1) {
            if(d >= 0) {
                y += sy;
                d += 2 * (dy - dx);
            } else {
                d += 2 * dy;
            }
            x += sx;
            sw->setPixel(x, y, line->borderColor);
        }
    } else {
        int d = 2 * dx - dy;
        while(y != y1) {
            if(d >= 0) {
                x += sx;
                d += 2 * (dx - dy);
            } else {
                d += 2 * dx;
            }
            y += sy;
            sw->setPixel(x, y, line->borderColor);
        }
    }
    sw->deactivate();
}

void Line_Midpoint_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Line_Midpoint_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());
    this->runAlgorithm(line, sw);
}
