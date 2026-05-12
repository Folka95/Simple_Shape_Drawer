#include "ellipse_midpoint_drawing_algorithm.h"
#include <bits/stdc++.h>

Ellipse_MidPoint_DrawingAlgorithm::Ellipse_MidPoint_DrawingAlgorithm() : DrawingAlgorithm() {
    
}


void Ellipse_MidPoint_DrawingAlgorithm::Draw4Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc + x, yc + y, color);
    sw->setPixel(xc + x, yc - y, color);
    sw->setPixel(xc - x, yc + y, color);
    sw->setPixel(xc - x, yc - y, color);
}

void Ellipse_MidPoint_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0 = ellipse->points[0];
    // Use your recently protected radii logic
    double a = abs(ellipse->points[1].x - p0.x); 
    double b = abs(ellipse->points[2].y - p0.y);

    if(a < 1.0 || b < 1.0) return;

    long long a2 = a * a;
    long long b2 = b * b;
    long long twoA2 = 2 * a2;
    long long twoB2 = 2 * b2;

    int x = 0;
    int y = b;
    
    sw->activate();

    // --- REGION 1: Slope < 1 ---
    // Initial decision parameter for Region 1
    double d1 = b2 - (a2 * b) + (0.25 * a2);
    long long dx = twoB2 * x;
    long long dy = twoA2 * y;

    while (dx < dy) {
        Draw4Points(sw, p0.x, p0.y, x, y, ellipse->borderColor);
        if (d1 < 0) {
            x++;
            dx += twoB2;
            d1 += dx + b2;
        } else {
            x++;
            y--;
            dx += twoB2;
            dy -= twoA2;
            d1 += dx - dy + b2;
        }
    }

    // --- REGION 2: Slope > 1 ---
    // Initial decision parameter for Region 2
    double d2 = (b2 * (x + 0.5) * (x + 0.5)) + (a2 * (y - 1) * (y - 1)) - (a2 * b2);

    while (y >= 0) {
        Draw4Points(sw, p0.x, p0.y, x, y, ellipse->borderColor);
        if (d2 > 0) {
            y--;
            dy -= twoA2;
            d2 += a2 - dy;
        } else {
            y--;
            x++;
            dx += twoB2;
            dy -= twoA2;
            d2 += dx - dy + a2;
        }
    }

    sw->deactivate();
}

void Ellipse_MidPoint_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE) return;
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone());
    this->runAlgorithm(ellipse, sw);
}