#include "line_dda_drawing_algorithm.h"
#include <bits/stdc++.h>

Line_DDA_DrawingAlgorithm::Line_DDA_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Line_DDA_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_LINE) {
        std::cerr << "Line_DDA_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);

    int dx = p0.dx(shape.points[1]);
    int dy = p0.dy(shape.points[1]);

    int steps = std::max(abs(dx), abs(dy));

    float m = shape.points[0].slope(shape.points[1]);;
    float invm = (dy != 0) ? (float)dx / dy : 1e9;

    sw->activate();
    sw->setPixel(p0.x, p0.y, shape.borderColor);

    float x = p0.x;
    float y = p0.y;

    for (int i = 0; i < steps; i++) {
        if (abs(m) > 1) {
            y += (dy > 0) ? 1 : -1;
            x = p0.x + (y - p0.y) * invm;
            sw->setPixel(round(x), y, shape.borderColor);
        } else {
            x += (dx > 0) ? 1 : -1;
            y = p0.y + (x - p0.x) * m;
            sw->setPixel(x, round(y), shape.borderColor);
        }
    }
    sw->deactivate();
}