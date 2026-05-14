// ============================================================
// fill_with_curve_filling_algorithm.cpp — Curve-Fill Algorithm
//
// CORE IDEA: Instead of filling with flat horizontal scanlines,
// fill the shape with CURVED Bézier lines that give a wavy,
// decorative fill pattern.
//
// TWO FILL MODES (depending on shape type):
//
//   RECTANGLE → fill_horizontal:
//     Sweeps from top to bottom. At each y row, draws a
//     Bézier curve from the left edge to the right edge,
//     with control points offset inward, creating a smooth
//     horizontal wave pattern.
//
//   SQUARE → fill_vertical:
//     Sweeps from left to right. At each x column, draws a
//     Hermite curve from the top edge to the bottom edge,
//     with vertical tangents for a smooth vertical wave.
//
// Both modes use the Hermite cubic formula internally,
// same as the spline drawing algorithm:
//   x(t) = h0*p0.x + h1*T0.x + h2*p3.x + h3*T1.x
//   y(t) = h0*p0.y + h1*T0.y + h2*p3.y + h3*T1.y
//
// The Bézier version converts control points to Hermite tangents:
//   T0 = 3*(c1 - p0)
//   T1 = 3*(p3 - c2)
// ============================================================

#include "fill_with_curve_filling_algorithm.h" // Header declaring this class
#include <cmath>       // std::hypot — for computing line length
#include <algorithm>   // std::max
#include <iostream>    // std::cerr

// Constructor
FillWithCurveFillingAlgorithm::FillWithCurveFillingAlgorithm()
    : FillingAlgorithm("FillWithCurveFillingAlgorithm") {}

// ── Draw one Hermite curve segment ────────────────────────────
// Same formula as the spline drawing algorithm.
// t0, t1 = tangent vectors. p0, p3 = endpoints.
void FillWithCurveFillingAlgorithm::drawCurveHermite(Shape* shape, Point t0, Point t1, Point p0, Point p3, ScreenWriter* sw, const Shape& clippingRegion) const {
    double dist = std::hypot(p3.x - p0.x, p3.y - p0.y); // Straight-line length of this segment
    int samples = std::max(2, static_cast<int>(dist * 2)); // Use 2 samples per pixel of length — smooth but not excessive
    double step = 1.0 / samples; // Parameter step per sample

    for (double t = 0.0; t <= 1.0; t += step) {
        double t2 = t * t;   // t² — reused in multiple basis functions
        double t3 = t2 * t;  // t³

        // Hermite basis functions
        double h0 =  2.0*t3 - 3.0*t2 + 1.0; // Weight for start point p0
        double h1 =      t3 - 2.0*t2 + t;   // Weight for start tangent t0
        double h2 = -2.0*t3 + 3.0*t2;        // Weight for end point p3
        double h3 =      t3 -     t2;        // Weight for end tangent t1

        // Blend endpoints and tangents to get the curve point at parameter t
        double xt = h0*p0.x + h1*t0.x + h2*p3.x + h3*t1.x;
        double yt = h0*p0.y + h1*t0.y + h2*p3.y + h3*t1.y;

        if (&clippingRegion == nullptr || clippingRegion.isInside(Point(xt, yt))) {
            sw->setPixel(round(xt), round(yt), shape->fillColor); // Draw if inside clipping region
        }
    }
}

// ── Convert Bézier control points to Hermite tangents and draw ─
// A cubic Bézier is defined by 4 points: p0, c1 (ctrl), c2 (ctrl), p3.
// Its tangents at p0 and p3 are: T0 = 3*(c1-p0), T1 = 3*(p3-c2)
void FillWithCurveFillingAlgorithm::drawCurveBezier(Shape* shape, Point c1, Point c2, Point p0, Point p3, ScreenWriter* sw, const Shape& clippingRegion) const {
    Point t0 = (c1 - p0) * 3.0; // Hermite start tangent derived from Bézier control point c1
    Point t1 = (p3 - c2) * 3.0; // Hermite end tangent derived from Bézier control point c2
    drawCurveHermite(shape, t0, t1, p0, p3, sw, clippingRegion); // Draw the curve using the Hermite formula
}

// ── Fill a RECTANGLE with horizontal Bézier curves ────────────
void FillWithCurveFillingAlgorithm::fill_horizontal(const Shape& shape, const Shape& clippingRegion, const Point& startPoint, ScreenWriter* sw) const {
    const RectangleShape* rectangle = dynamic_cast<const RectangleShape*>(&shape);
    if (!rectangle) {
        std::cerr << "FillWithCurveFillingAlgorithm::fill_horizontal: shape is not a RectangleShape\n";
        return;
    }

    int steps = rectangle->getHeight(); // Number of horizontal curves = height of the rectangle in pixels

    for (int i = 0; i <= steps; i++) {
        double s = static_cast<double>(i) / steps; // Normalized position from 0 (top) to 1 (bottom)

        // Interpolate left and right edge points at this height using lerp
        Point left  = rectangle->getTopLeft().lerp(rectangle->getBottomLeft(),  s); // Left endpoint of this curve
        Point right = rectangle->getTopRight().lerp(rectangle->getBottomRight(), s); // Right endpoint of this curve

        double offset = (right.x - left.x) / 4.0; // Control point offset = 1/4 of the width
                                                    // Determines how "wavy" the curve is

        // Control points: offset inward from each endpoint to create an S-shaped wave
        Point c1 = left  + Point(offset, 0.0); // Left control point: shifted right from left endpoint
        Point c2 = right - Point(offset, 0.0); // Right control point: shifted left from right endpoint

        drawCurveBezier(const_cast<Shape*>(&shape), c1, c2, left, right, sw, clippingRegion); // Draw the Bézier curve
    }
}

// ── Fill a SQUARE with vertical Hermite curves ────────────────
void FillWithCurveFillingAlgorithm::fill_vertical(const Shape& shape, const Shape& clippingRegion, const Point& startPoint, ScreenWriter* sw) const {
    const Square* square = dynamic_cast<const Square*>(&shape);
    if (!square) {
        std::cerr << "FillWithCurveFillingAlgorithm::fill_vertical: shape is not a Square\n";
        return;
    }

    int steps = square->getSideLength();           // Number of vertical curves = side length in pixels
    double tangentStrength = steps * 0.15;         // Tangent magnitude = 15% of side length
                                                    // Larger = more exaggerated curve; smaller = closer to straight lines

    for (int i = 0; i <= steps; i++) {
        double s = static_cast<double>(i) / steps; // Normalized position from 0 (left) to 1 (right)

        // Interpolate top and bottom edge points at this x position using lerp
        Point top    = square->getTopLeft().lerp(square->getTopRight(),    s); // Top endpoint of this curve
        Point bottom = square->getBottomLeft().lerp(square->getBottomRight(), s); // Bottom endpoint

        // Both tangents point straight down (positive y) with magnitude = tangentStrength
        // This creates gently bowed vertical curves rather than straight vertical lines
        Point t0(0.0, tangentStrength); // Outgoing tangent at top: pointing downward
        Point t1(0.0, tangentStrength); // Incoming tangent at bottom: also pointing downward

        drawCurveHermite(const_cast<Shape*>(&shape), t0, t1, top, bottom, sw, clippingRegion); // Draw the Hermite curve
    }
}

// ── Public entry point ────────────────────────────────────────
void FillWithCurveFillingAlgorithm::fill(const Shape& shape, const Shape& clippingRegion, const Point& startPoint, ScreenWriter* sw) const {
    if (shape.getType() == SHAPE_SQUARE) {
        sw->activate();
        fill_vertical(shape, clippingRegion, startPoint, sw); // Square → vertical curve fill
        sw->deactivate();
    } else if (shape.getType() == SHAPE_RECTANGLE) {
        sw->activate();
        fill_horizontal(shape, clippingRegion, startPoint, sw); // Rectangle → horizontal curve fill
        sw->deactivate();
    } else {
        std::cerr << "FillWithCurveFillingAlgorithm::fill: unsupported shape" << std::endl;
        // This algorithm only supports squares and rectangles
    }
}
