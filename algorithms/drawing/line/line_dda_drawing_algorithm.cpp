#include "line_dda_drawing_algorithm.h"
#include <bits/stdc++.h>

Line_DDA_DrawingAlgorithm::Line_DDA_DrawingAlgorithm() : DrawingAlgorithm("Line_DDA_DrawingAlgorithm") {

}

void Line_DDA_DrawingAlgorithm::runAlgorithm(Line* line, ScreenWriter *sw) const {
    Point p0(line->points[0]);
    Point p1(line->points[1]);

    int dx = p0.dx(line->points[1]);
    int dy = p0.dy(line->points[1]);

    sw->activate();
    if(dx == 0 && dy == 0) {
        sw->setPixel(p0.x, p0.y, line->borderColor);
        return;
    }
    sw->setPixel(p0.x, p0.y, line->borderColor);
    if(abs(dx)>=abs(dy)) {
        int x=p0.x,xinc= dx>0?1:-1;
        double y=p0.y,yinc=(double)dy/dx*xinc;
        while(x!=p1.x) {
            x+=xinc;
            y+=yinc;
            sw->setPixel(x, round(y), line->borderColor);
        }
    } else {
        int y=p0.y,yinc= dy>0?1:-1;
        double x=p0.x,xinc=(double)dx/dy*yinc;
        while(y!=p1.y) {
            x+=xinc;
            y+=yinc;
            sw->setPixel(round(x), y, line->borderColor);
        }
    }
    sw->deactivate();
}

void Line_DDA_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Line_DDA_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());
    this->runAlgorithm(line, sw);
}
