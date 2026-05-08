#include "circle_direct_drawing_algorithm.h"
#include <bits/stdc++.h>

Circle_Direct_DrawingAlgorithm::Circle_Direct_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Circle_Direct_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color);
    sw->setPixel(xc+x, yc-y, color);
    sw->setPixel(xc-x, yc+y, color);
    sw->setPixel(xc-x, yc-y, color);
    sw->setPixel(xc+y, yc+x, color);
    sw->setPixel(xc+y, yc-x, color);
    sw->setPixel(xc-y, yc+x, color);
    sw->setPixel(xc-y, yc-x, color);
}

void Circle_Direct_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Direct_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_Direct_DrawingAlgorithm::draw : radius must be greater than 0" << std::endl;
        return;
    }
    
    int x=0,y=radius;
    int Rsq=radius*radius;
    sw->activate();
    Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    while(x<y) {
    x++;
    y=round(sqrt((double)(Rsq-x*x)));
    Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    }
    sw->deactivate();
}