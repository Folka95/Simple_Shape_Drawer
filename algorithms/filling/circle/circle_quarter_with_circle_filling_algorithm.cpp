#include "circle_quarter_with_circle_filling_algorithm.h"

Circle_QuarterWithCircle_FillingAlgorithm::Circle_QuarterWithCircle_FillingAlgorithm() : FillingAlgorithm() {

}

void Circle_QuarterWithCircle_FillingAlgorithm::drawQuarterCircle(double xc, double yc, double radius, int q, ScreenWriter *sw, COLORREF color) const {
    double step = 1.0 / radius;

    double start = 0;
    double end = M_PI/2;


    if (q == 1) {
        start = 0;
        end = M_PI/2;
    } else if (q == 2) {
        start = M_PI/2;
        end = M_PI;
    } else if (q == 3) {
        start = M_PI;
        end = 3* M_PI/2;
    } else if (q == 4) {
        start = 3* M_PI/2;
        end = 2*M_PI;
    }

    for (double radian = start; radian < end; radian += step) {
        double x = (xc + radius * cos(radian));
        double y = (yc + radius * sin(radian));
        sw->setPixel(round(x), round(y), color);
    }
}

void Circle_QuarterWithCircle_FillingAlgorithm::fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {

    int q = circle.scanQuarter(startPoint);

    if (q == 0 || q == 5) {
        std::cerr << "Circle_QuarterWithCircle_FillingAlgorithm::fill_helper : Please Choose a valid quarter" << std::endl;
        return;
    }

    Point center = circle.getCenter();
    double radius = circle.getRadius();

    for (double r = 0; r < radius; r+= 0.3) {
        drawQuarterCircle(center.x , center.y, r, q, sw, circle.fillColor);
    }
}

void Circle_QuarterWithCircle_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_QuarterWithCircle_FillingAlgorithm::fill : Region must be a Circle" << std::endl;
        return;
    }
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (!circle) return;

    sw->activate();
    fill_helper(*circle, clippingRegion, startPoint, sw);
    sw->deactivate();
}