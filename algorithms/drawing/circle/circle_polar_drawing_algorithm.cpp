#include "circle_polar_drawing_algorithm.h"
#include <bits/stdc++.h>

Circle_Polar_DrawingAlgorithm::Circle_Polar_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Circle_Polar_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Polar_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_Polar_DrawingAlgorithm::draw : radius must be greater than 0" << std::endl;
        return;
    }
    
    double step = 1.0 / radius;
    sw->activate();
    for (double radian = 0; radian < 2*3.14159265358979323846; radian += step) {
        double x = (p0.x + radius * cos(radian));
        double y = (p0.y + radius * sin(radian));
        sw->setPixel(round(x), round(y), shape.borderColor);
    }
    sw->deactivate();
}