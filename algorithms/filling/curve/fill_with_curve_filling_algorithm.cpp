#include "fill_with_curve_filling_algorithm.h"
#include <cmath>
#include <algorithm>
#include <iostream>

FillWithCurveFillingAlgorithm::FillWithCurveFillingAlgorithm() : FillingAlgorithm("FillWithCurveFillingAlgorithm") {}

void FillWithCurveFillingAlgorithm::drawCurveHermite(Shape* shape, Point t0, Point t1, Point p0, Point p3, ScreenWriter* sw, const Shape& clippingRegion) const {
    // Use 2 samples per pixel
    double dist = std::hypot(p3.x - p0.x, p3.y - p0.y);
    int samples = std::max(2, static_cast<int>(dist * 2));
    double step = 1.0 / samples;

    for (double t = 0.0; t <= 1.0; t += step) {
        double t2 = t * t;
        double t3 = t2 * t;

        double h0 = 2.0*t3 - 3.0*t2 + 1.0;
        double h1 = t3 - 2.0*t2 + t;
        double h2 = -2.0 * t3 + 3.0*t2;
        double h3 = t3 -t2;

        double xt = h0*p0.x + h1*t0.x + h2*p3.x + h3*t1.x;
        double yt = h0*p0.y + h1*t0.y + h2*p3.y + h3*t1.y;

        if (&clippingRegion == nullptr || clippingRegion.isInside(Point(xt, yt))) {
            sw->setPixel(round(xt), round(yt), shape->fillColor);
        }
    }
}

void FillWithCurveFillingAlgorithm::drawCurveBezier(Shape* shape, Point c1, Point c2, Point p0, Point p3, ScreenWriter* sw, const Shape& clippingRegion) const {
    Point t0 = (c1 - p0) * 3.0;
    Point t1 = (p3 - c2) * 3.0;
    drawCurveHermite(shape, t0, t1, p0, p3, sw, clippingRegion);
}


void FillWithCurveFillingAlgorithm::fill_horizontal(const Shape&   shape, const Shape&   clippingRegion, const Point&   startPoint, ScreenWriter*  sw) const {
    const RectangleShape* rectangle = dynamic_cast<const RectangleShape*>(&shape);
    if (!rectangle) {
        std::cerr << "FillWithCurveFillingAlgorithm::fill_horizontal: shape is not a RectangleShape\n";
        return;
    }

    int steps = rectangle->getHeight();

    for (int i = 0; i <= steps; i++) {
        double s = static_cast<double>(i) / steps;

        Point left  = rectangle->getTopLeft().lerp(rectangle->getBottomLeft(),  s);
        Point right = rectangle->getTopRight().lerp(rectangle->getBottomRight(), s);

        double offset = (right.x - left.x) / 4.0;

        Point c1 = left  + Point(offset, 0.0);
        Point c2 = right - Point(offset, 0.0);

        drawCurveBezier(const_cast<Shape*>(&shape), c1, c2, left, right, sw, clippingRegion);
    }
}


void FillWithCurveFillingAlgorithm::fill_vertical(const Shape&  shape, const Shape&  clippingRegion, const Point&  startPoint, ScreenWriter* sw) const {
    const Square* square = dynamic_cast<const Square*>(&shape);
    if (!square) {
        std::cerr << "FillWithCurveFillingAlgorithm::fill_vertical: shape is not a Square\n";
        return;
    }

    int steps = square->getSideLength();
    double tangentStrength = steps * 0.15;

    for (int i = 0; i <= steps; i++) {
        double s = static_cast<double>(i) / steps;

        Point top = square->getTopLeft().lerp(square->getTopRight(), s);
        Point bottom = square->getBottomLeft().lerp(square->getBottomRight(), s);

        Point t0(0.0, tangentStrength);
        Point t1(0.0, tangentStrength);

        drawCurveHermite(const_cast<Shape*>(&shape), t0, t1, top, bottom, sw, clippingRegion);
    }
}

void FillWithCurveFillingAlgorithm::fill(const Shape& shape, const Shape& clippingRegion, const Point& startPoint, ScreenWriter* sw) const {
    if (shape.getType() == SHAPE_SQUARE) {
        sw->activate();
        fill_vertical(shape, clippingRegion, startPoint, sw);
        sw->deactivate();
    } else if (shape.getType() == SHAPE_RECTANGLE) {
        sw->activate();
        fill_horizontal(shape, clippingRegion, startPoint, sw);
        sw->deactivate();
    } else {
        std::cerr << "FillWithCurveFillingAlgorithm::fill: unsupported shape" << std::endl;
    }
}