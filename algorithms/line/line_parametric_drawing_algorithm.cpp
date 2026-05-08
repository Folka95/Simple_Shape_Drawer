#include "line_parametric_drawing_algorithm.h"
#include <bits/stdc++.h>

Line_Parametric_DrawingAlgorithm::Line_Parametric_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Line_Parametric_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {

    if(shape.getType() != SHAPE_LINE) {
        std::cerr << "Line_Parametric_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }

    Point p0(shape.points[0]);
    Point p1(shape.points[1]);

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    int steps = std::max(abs(dx), abs(dy));

    sw->activate();

    for(int i = 0; i <= steps; i++) {
        double t = (double)i / steps;
        double x = p0.x + dx * t;
        double y = p0.y + dy * t;
        sw->setPixel(round(x), round(y), shape.borderColor);
    }

    sw->deactivate();
}