#include "circle_midpoint_drawing_algorithm.h"
#include <bits/stdc++.h>

Circle_MidPoint_DrawingAlgorithm::Circle_MidPoint_DrawingAlgorithm() : DrawingAlgorithm() {
    
}


void Circle_MidPoint_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color);
    sw->setPixel(xc+x, yc-y, color);
    sw->setPixel(xc-x, yc+y, color);
    sw->setPixel(xc-x, yc-y, color);
    sw->setPixel(xc+y, yc+x, color);
    sw->setPixel(xc+y, yc-x, color);
    sw->setPixel(xc-y, yc+x, color);
    sw->setPixel(xc-y, yc-x, color);
}

void Circle_MidPoint_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_MidPoint_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_MidPoint_DrawingAlgorithm::draw : radius must be greater than 0" << std::endl;
        return;
    }
    
    int x=0,y=radius;
    int d=1-radius;
    sw->activate();
    Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    while(x<y) {
        if(d<0) {
            d+=2*x+2;
        }
        else {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    }
    sw->deactivate();
}