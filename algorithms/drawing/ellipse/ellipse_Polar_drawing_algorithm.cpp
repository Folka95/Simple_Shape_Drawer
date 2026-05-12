#include "ellipse_polar_drawing_algorithm.h"
#include <bits/stdc++.h>

Ellipse_Polar_DrawingAlgorithm::Ellipse_Polar_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Ellipse_Polar_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0(ellipse->points[0]);
    Point p1(ellipse->points[1]);
    Point p2(ellipse->points[2]);
    double a = p0.euclideanDistance(p1);
    double b = p0.euclideanDistance(p2);

    if(a < 1.0 || b < 1.0) {
        std::cerr << "Ellipse_Polar_DrawingAlgorithm::runAlgorithm : semi-axes must be greater than 0" << std::endl;
        return;
    }

    double step = 1.0 / std::max(a, b);
    sw->activate();
    for (double radian = 0; radian < 2*3.14159265358979323846; radian += step) {
        double x = (p0.x + a * cos(radian));
        double y = (p0.y + b * sin(radian));
        sw->setPixel(round(x), round(y), ellipse->borderColor);
    }
    sw->deactivate();
}

void Ellipse_Polar_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE) {
        std::cerr << "Ellipse_Polar_DrawingAlgorithm::draw : shape to draw must be Ellipse" << std::endl;
        return;
    }
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone());
    this->runAlgorithm(ellipse, sw);
}