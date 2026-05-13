#include "ellipse_direct_drawing_algorithm.h"
#include <bits/stdc++.h>

Ellipse_Direct_DrawingAlgorithm::Ellipse_Direct_DrawingAlgorithm() : DrawingAlgorithm("Ellipse_Direct_DrawingAlgorithm") {

}

// Change to 4-point symmetry for Ellipses
void Ellipse_Direct_DrawingAlgorithm::Draw4Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc + x, yc + y, color);
    sw->setPixel(xc + x, yc - y, color);
    sw->setPixel(xc - x, yc + y, color);
    sw->setPixel(xc - x, yc - y, color);
}
void Ellipse_Direct_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0 = ellipse->points[0]; // Center
    double a = p0.euclideanDistance(ellipse->points[1]); // Horizontal semi-axis
    double b = p0.euclideanDistance(ellipse->points[2]); // Vertical semi-axis

    if(a < 1.0 || b < 1.0) return;

    sw->activate();
    
    // REGION 1: Iterate x from 0 until the slope becomes steeper than 1
    // The slope is > 1 when (b^2 * x) / (a^2 * y) > 1
    int x = 0;
    double y = b;
    while ((b * b * x) <= (a * a * y)) {
        Draw4Points(sw, p0.x, p0.y, x, (int)round(y), ellipse->borderColor);
        x++;
        y = b * sqrt(1.0 - (double)(x * x) / (a * a));
    }

    // REGION 2: Iterate y from 0 until the slope becomes less than 1
    // This fills the "gaps" at the steep sides of the ellipse
    int iy = 0;
    double ix = a;
    while ((a * a * iy) < (b * b * ix)) {
        Draw4Points(sw, p0.x, p0.y, (int)round(ix), iy, ellipse->borderColor);
        iy++;
        ix = a * sqrt(1.0 - (double)(iy * iy) / (b * b));
    }
    sw->deactivate();
}

void Ellipse_Direct_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE)  {
        std::cerr << "Ellipse_Direct_DrawingAlgorithm::draw : shape to draw must be Ellipse" << std::endl;
        return;
    }
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone());
    this->runAlgorithm(ellipse, sw);
}
