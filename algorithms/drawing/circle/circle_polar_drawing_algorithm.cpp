#include "circle_polar_drawing_algorithm.h"
#include <bits/stdc++.h>

Circle_Polar_DrawingAlgorithm::Circle_Polar_DrawingAlgorithm() : DrawingAlgorithm("Circle_Polar_DrawingAlgorithm") {

}

void Circle_Polar_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]);
    Point p1(circle->points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_Polar_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }
    
    double step = 1.0 / radius;
    sw->activate();
    for (double radian = 0; radian < 2*3.14159265358979323846; radian += step) {
        double x = (p0.x + radius * cos(radian));
        double y = (p0.y + radius * sin(radian));
        sw->setPixel(round(x), round(y), circle->borderColor);
    }
    sw->deactivate();
}

void Circle_Polar_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Polar_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone());
    this->runAlgorithm(circle, sw);
}
