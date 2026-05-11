//
// Created by frasa on 5/11/2026.
//
#include "rectangle_line_clipping_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"

union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
    return out;
}

void Rectangle_Line_ClippingAlgorithm::VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const{
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void Rectangle_Line_ClippingAlgorithm::HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const{
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}


void Rectangle_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const {
    Line *line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}


Rectangle_Line_ClippingAlgorithm::Rectangle_Line_ClippingAlgorithm() : ClippingAlgorithm() {}

void Rectangle_Line_ClippingAlgorithm::clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_LINE) {
        std::cerr << "rectangleLineClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (region.getType() != SHAPE_RECTANGLE) {
        std::cerr << "rectangleLineClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }
    int xleft = region.points[0].x, ytop = region.points[0].y, xright = region.points[1].x, ybottom = region.points[1].y;

    double x1 = shape.points[0].x, y1 = shape.points[0].y, x2 = shape.points[1].x, y2 = shape.points[1].y;

    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi;
        if (out1.All) {
            if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        } else {
            if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        }
    }

    if (!out1.All && !out2.All) {
        Point p1(x1, y1);
        Point p2(x2, y2);
        drawLine(p1, p2, sw);
    }


}
