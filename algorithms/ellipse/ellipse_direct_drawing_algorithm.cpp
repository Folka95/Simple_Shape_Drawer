#include "ellipse_direct_drawing_algorithm.h"
#include <bits/stdc++.h>

Ellipse_Direct_DrawingAlgorithm::Ellipse_Direct_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Ellipse_Direct_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color);
    sw->setPixel(xc+x, yc-y, color);
    sw->setPixel(xc-x, yc+y, color);
    sw->setPixel(xc-x, yc-y, color);
    sw->setPixel(xc+y, yc+x, color);
    sw->setPixel(xc+y, yc-x, color);
    sw->setPixel(xc-y, yc+x, color);
    sw->setPixel(xc-y, yc-x, color);
}

void Ellipse_Direct_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_ELLIPSE) {
        std::cerr << "Ellipse_Direct_DrawingAlgorithm::draw : shape to draw must be Ellipse" << std::endl;
        return;
    }
    Point p0(shape.points[0]);
    Point p1(shape.points[1]);
    Point p2(shape.points[2]);
    double a = p0.euclideanDistance(p1);
    double b = p0.euclideanDistance(p2);

    if(a < 1.0 || b < 1.0) {
        std::cerr << "Ellipse_Direct_DrawingAlgorithm::draw : semi-axes must be greater than 0" << std::endl;
        return;
    }

    int x=0,y=b;
    int Rsq=a*a*b*b;
    sw->activate();
    Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    while(x<y) {
        x++;
        y=round(sqrt((double)(Rsq-x*x)));
        Draw8Points(sw, shape.points[0].x, shape.points[0].y, x, y, shape.borderColor);
    }
    sw->deactivate();
}