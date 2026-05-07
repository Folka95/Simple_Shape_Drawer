#include "circle_line_clipping_algorithm.h"

Circle_Line_ClippingAlgorithm::Circle_Line_ClippingAlgorithm() : ClippingAlgorithm() {

}

void Circle_Line_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_LINE) {
        std::cerr << "lineClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if(region.getType() != SHAPE_CIRCLE) {
        std::cerr << "lineClippingAlgorithm::clip : region must be Circle" << std::endl;
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
    if(region.isInside(p0)) {
        sw->setPixel(p0.x, p0.y, shape.borderColor);
    }

    float x = p0.x;
    float y = p0.y;

    for (int i = 0; i < steps; i++) {
        Point p;
        if (abs(m) > 1) {
            y += (dy > 0) ? 1 : -1;
            x = p0.x + (y - p0.y) * invm;
            p.x = round(x);
            p.y = y;
        } else {
            x += (dx > 0) ? 1 : -1;
            y = p0.y + (x - p0.x) * m;
            p.x = x;
            p.y = round(y);
        }
        if(region.isInside(p)) {
            sw->setPixel(p.x, p.y, shape.borderColor);
        }
    }
    sw->deactivate();
}