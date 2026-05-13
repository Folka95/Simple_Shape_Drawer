#include "circle_line_clipping_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void Circle_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const {
    Line *line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}

Circle_Line_ClippingAlgorithm::Circle_Line_ClippingAlgorithm() : ClippingAlgorithm("Circle_Line_ClippingAlgorithm") {
}

void Circle_Line_ClippingAlgorithm::runAlgorithm(Line *line, Circle *circle, ScreenWriter *sw) const {
    if (line == nullptr || circle == nullptr || line->points.size() < 2 || circle->points.size() < 2) {
        return;
    }

    const double x0 = line->points[0].x;
    const double y0 = line->points[0].y;
    const double x1 = line->points[1].x;
    const double y1 = line->points[1].y;

    const double cx = circle->points[0].x;
    const double cy = circle->points[0].y;
    const double r = circle->points[0].euclideanDistance(circle->points[1]);
    constexpr double eps = 1e-9;
    if (r <= eps) {
        return;
    }
    const double r2 = r * r;

    const double dx = x1 - x0;
    const double dy = y1 - y0;
    const double A = dx * dx + dy * dy;

    auto inside = [&](double px, double py) {
        const double vx = px - cx;
        const double vy = py - cy;
        return vx * vx + vy * vy <= r2 + eps;
    };

    if (A < eps) {
        if (inside(x0, y0)) {
            drawLine(Point(x0, y0), Point(x0, y0), sw);
        }
        return;
    }

    const double fx = x0 - cx;
    const double fy = y0 - cy;
    const double B = 2.0 * (fx * dx + fy * dy);
    const double C = fx * fx + fy * fy - r2;

    const double disc = B * B - 4.0 * A * C;
    const bool in0 = inside(x0, y0);
    const bool in1 = inside(x1, y1);

    if (disc < -eps) {
        if (in0 && in1) {
            drawLine(line->points[0], line->points[1], sw);
        }
        return;
    }

    const double discClamped = std::max(0.0, disc);
    const double sqrtD = std::sqrt(discClamped);
    double t1 = (-B - sqrtD) / (2.0 * A);
    double t2 = (-B + sqrtD) / (2.0 * A);
    if (t1 > t2) {
        std::swap(t1, t2);
    }

    const double tStart = std::max(0.0, t1);
    const double tEnd = std::min(1.0, t2);

    if (tStart > tEnd + eps) {
        if (in0 && in1) {
            drawLine(line->points[0], line->points[1], sw);
        }
        return;
    }

    const Point pa(x0 + tStart * dx, y0 + tStart * dy);
    const Point pb(x0 + tEnd * dx, y0 + tEnd * dy);
    drawLine(pa, pb, sw);
}

void Circle_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    Line *line = dynamic_cast<Line *>(inputShape.clone());
    Circle *circle = dynamic_cast<Circle *>(inputRegion.clone());
    this->runAlgorithm(line, circle, sw);
}
