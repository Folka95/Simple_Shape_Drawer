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

void Circle_Direct_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]);
    Point p1(circle->points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_Direct_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }
    
    int x=0,y=radius;
    int Rsq=radius*radius;
    sw->activate();
    Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor);
    while(x<y) {
        x++;
        y=round(sqrt((double)(Rsq-x*x)));
        Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor);
    }
    sw->deactivate();
}

void Circle_Direct_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Direct_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone());
    this->runAlgorithm(circle, sw);
}