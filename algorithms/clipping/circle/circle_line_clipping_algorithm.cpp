// ============================================================
// circle_line_clipping_algorithm.cpp — Line Clipping Against a Circle
//
// CORE IDEA: Find the portion of a line segment that lies
// INSIDE the circular clipping region, and draw only that.
//
// METHOD: Substitute the parametric line P(t) = P0 + t*(P1-P0)
// into the circle equation (x-cx)² + (y-cy)² = r² to get a
// quadratic in t:
//   A*t² + B*t + C = 0
// where:
//   A = |P1-P0|² = dx²+dy²
//   B = 2 * ((P0-C) · (P1-P0))   (dot product)
//   C = |P0-C|² - r²
//
// The discriminant  disc = B² - 4AC tells us:
//   disc < 0  → no intersection → line is entirely inside or outside circle
//   disc ≥ 0  → two t values where line crosses circle boundary
//
// We clamp t to [0,1] to stay within the segment.
// The visible segment is from P(tStart) to P(tEnd).
//
// Edge cases handled:
//   - Zero-length segment (A ≈ 0)
//   - Both endpoints inside (no intersection but still draw)
//   - Entire segment outside (nothing to draw)
// ============================================================

#include "circle_line_clipping_algorithm.h"                        // Header declaring this class
#include "../../../core/shapes/line.h"                              // Line shape — for drawing the clipped segment
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"     // Midpoint line drawer
#include <algorithm>  // std::max, std::min, std::swap
#include <cmath>      // std::sqrt
#include <iostream>   // std::cerr

// ── Helper: draw the surviving line segment ───────────────────
void Circle_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const {
    Line *line = new Line();  // Create temporary Line shape
    line->addPoint(p1);       // First endpoint of the clipped segment
    line->addPoint(p2);       // Second endpoint
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw); // Draw using Bresenham midpoint
}

// Constructor
Circle_Line_ClippingAlgorithm::Circle_Line_ClippingAlgorithm()
    : ClippingAlgorithm("Circle_Line_ClippingAlgorithm") {
}

// ── Core algorithm ────────────────────────────────────────────
void Circle_Line_ClippingAlgorithm::runAlgorithm(Line *line, Circle *circle, ScreenWriter *sw) const {
    if (line == nullptr || circle == nullptr || line->points.size() < 2 || circle->points.size() < 2) {
        return; // Null or incomplete shapes — abort
    }

    // ── Extract line endpoints ────────────────────────────────
    const double x0 = line->points[0].x;  // Start x
    const double y0 = line->points[0].y;  // Start y
    const double x1 = line->points[1].x;  // End x
    const double y1 = line->points[1].y;  // End y

    // ── Extract circle parameters ─────────────────────────────
    const double cx = circle->points[0].x; // Circle center x
    const double cy = circle->points[0].y; // Circle center y
    const double r  = circle->points[0].euclideanDistance(circle->points[1]); // Radius

    constexpr double eps = 1e-9; // Epsilon for floating-point comparisons (avoids divide-by-zero / false negatives)

    if (r <= eps) return; // Zero-radius circle — nothing to clip against

    const double r2 = r * r; // Pre-compute r² to avoid repeated squaring

    // ── Direction vector of the line ──────────────────────────
    const double dx = x1 - x0; // x component of direction vector P1-P0
    const double dy = y1 - y0; // y component of direction vector

    const double A = dx * dx + dy * dy; // A = |P1-P0|² = squared length of the segment

    // Lambda: test if point (px,py) is inside the circle
    auto inside = [&](double px, double py) {
        const double vx = px - cx; // x offset from center
        const double vy = py - cy; // y offset from center
        return vx * vx + vy * vy <= r2 + eps; // Inside if distance² ≤ r² (with epsilon tolerance)
    };

    // ── Edge case: zero-length segment ───────────────────────
    if (A < eps) {
        // The segment is a single point — draw it only if it's inside the circle
        if (inside(x0, y0)) {
            drawLine(Point(x0, y0), Point(x0, y0), sw);
        }
        return;
    }

    // ── Build the quadratic At² + Bt + C = 0 ─────────────────
    const double fx = x0 - cx; // x offset from circle center to line start
    const double fy = y0 - cy; // y offset from circle center to line start
    const double B  = 2.0 * (fx * dx + fy * dy); // B = 2*(P0-C)·direction — the dot product term
    const double C  = fx * fx + fy * fy - r2;     // C = |P0-C|² - r² — negative if P0 is inside circle

    const double disc = B * B - 4.0 * A * C; // Discriminant: positive → two intersections, zero → tangent, negative → no intersection

    const bool in0 = inside(x0, y0); // Is the start point inside the circle?
    const bool in1 = inside(x1, y1); // Is the end   point inside the circle?

    if (disc < -eps) {
        // No real intersections with the circle boundary
        if (in0 && in1) {
            // Both endpoints inside → the entire segment is inside → draw it all
            drawLine(line->points[0], line->points[1], sw);
        }
        // If neither inside → entire segment is outside → draw nothing
        return;
    }

    // ── Solve for the two t-values where the line meets the circle ──
    const double discClamped = std::max(0.0, disc); // Clamp to 0 to avoid sqrt of tiny negative due to floating-point
    const double sqrtD = std::sqrt(discClamped);    // Square root of discriminant
    double t1 = (-B - sqrtD) / (2.0 * A); // Smaller root (entry point)
    double t2 = (-B + sqrtD) / (2.0 * A); // Larger root  (exit point)

    if (t1 > t2) std::swap(t1, t2); // Ensure t1 ≤ t2 (should already be true, but guard for numerical safety)

    // Clamp to [0,1] — we only care about the portion within the segment, not outside the endpoints
    const double tStart = std::max(0.0, t1); // Start of the visible portion (no earlier than P0)
    const double tEnd   = std::min(1.0, t2); // End   of the visible portion (no later than P1)

    if (tStart > tEnd + eps) {
        // The circle-intersection interval [t1,t2] doesn't overlap [0,1] → segment is entirely outside
        if (in0 && in1) {
            // But wait — both endpoints inside means the WHOLE segment is inside (circle wraps around)
            drawLine(line->points[0], line->points[1], sw);
        }
        return;
    }

    // ── Compute the clipped endpoints and draw ────────────────
    const Point pa(x0 + tStart * dx, y0 + tStart * dy); // Point on the segment at t = tStart (enters or starts inside)
    const Point pb(x0 + tEnd   * dx, y0 + tEnd   * dy); // Point on the segment at t = tEnd   (exits or ends inside)
    drawLine(pa, pb, sw); // Draw the surviving visible portion
}

// ── Public entry point ────────────────────────────────────────
void Circle_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Line_ClippingAlgorithm::clip : region must be Circle" << std::endl;
        return;
    }
    Line   *line   = dynamic_cast<Line*>  (inputShape.clone());  // Clone + cast to Line*
    Circle *circle = dynamic_cast<Circle*>(inputRegion.clone()); // Clone + cast to Circle*
    this->runAlgorithm(line, circle, sw);
}
