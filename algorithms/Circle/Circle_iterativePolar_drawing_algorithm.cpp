#include "circle_iterativePolar_drawing_algorithm.h"
#include <bits/stdc++.h>

Circle_iterativePolar_DrawingAlgorithm::Circle_iterativePolar_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Circle_iterativePolar_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_iterativePolar_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
    
    if(radius < 1.0) {
        std::cerr << "Circle_iterativePolar_DrawingAlgorithm::draw : radius must be greater than 0" << std::endl;
        return;
    }
    
    double deltaTheta = 1.0 / radius;
    double cosDelta = cos(deltaTheta);
    double sinDelta = sin(deltaTheta);
    double x = radius;
    double y = 0;
    int d = (int)(cos(3.14159265358979323846/4) * radius);
    sw->activate();
    for (double radian = 0; radian < 3.14159265358979323846/4 ; radian += deltaTheta) {
        Point octet1 = { (p0.x + x), (p0.y + y) };
        Point octet2 = { (p0.x + x), (p0.y - y) };
        Point octet3 = { (p0.x - x), (p0.y + y) };
        Point octet4 = { (p0.x - x), (p0.y - y) };
        Point octet5 = { (p0.x + y), (p0.y + x) };
        Point octet6 = { (p0.x + y), (p0.y - x) };
        Point octet7 = { (p0.x - y), (p0.y + x) };
        Point octet8 = { (p0.x - y), (p0.y - x) };
        sw->setPixel(round(octet1.x), round(octet1.y), shape.borderColor);
        sw->setPixel(round(octet2.x), round(octet2.y), shape.borderColor);
        sw->setPixel(round(octet3.x), round(octet3.y), shape.borderColor);
        sw->setPixel(round(octet4.x), round(octet4.y), shape.borderColor);
        sw->setPixel(round(octet5.x), round(octet5.y), shape.borderColor);
        sw->setPixel(round(octet6.x), round(octet6.y), shape.borderColor);
        sw->setPixel(round(octet7.x), round(octet7.y), shape.borderColor);
        sw->setPixel(round(octet8.x), round(octet8.y), shape.borderColor);
        double newX = x * cosDelta - y * sinDelta;
        double newY = x * sinDelta + y * cosDelta;
        x = newX;
        y = newY;
    }
    sw->deactivate();
}