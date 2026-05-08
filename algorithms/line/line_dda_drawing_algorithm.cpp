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

    // int steps = std::max(abs(dx), abs(dy));
    // float m = shape.points[0].slope(shape.points[1]);;
    // float invm = (dy != 0) ? (float)dx / dy : 1e9;

    sw->activate();
    if(dx == 0 && dy == 0) {
        sw->setPixel(p0.x, p0.y, shape.borderColor);
        return;
    }
    sw->setPixel(p0.x, p0.y, shape.borderColor);
    if(abs(dx)>=abs(dy)) {
        int x=p0.x,xinc= dx>0?1:-1;
        double y=p0.y,yinc=(double)dy/dx*xinc;
        while(x!=p1.x) {
            x+=xinc;
            y+=yinc;
            sw->setPixel(x, round(y), shape.borderColor);
        }
    } else {
        int y=p0.y,yinc= dy>0?1:-1;
        double x=p0.x,xinc=(double)dx/dy*yinc;
        while(y!=p1.y) {
            x+=xinc;
            y+=yinc;
            sw->setPixel(round(x), y, shape.borderColor);
        }
    }
    sw->deactivate();
}