// ============================================================
// ellipse_direct_drawing_algorithm.cpp — Direct Ellipse Algorithm
//
// CORE IDEA: Uses the ellipse equation directly:
//   (x/a)² + (y/b)² = 1  →  y = b * sqrt(1 - x²/a²)
//
// Like the direct circle algorithm, we compute one coordinate
// from the equation and use 4-fold symmetry to mirror it to all
// four quadrants.
//
// Because the curve slope changes from flat to steep, we need
// TWO REGIONS (same concept as the midpoint algorithm):
//   REGION 1: slope < 1 (curve is flatter) — step x, compute y
//             Continue while b²x ≤ a²y
//   REGION 2: slope > 1 (curve is steeper) — step y, compute x
//             Continue while a²y < b²x
//
// The transition between regions happens at the point where
// the tangent slope equals 1 (the "45°" point of the ellipse).
// Using two regions avoids gaps in the drawn boundary.
//
// Cost: two sqrt() calls per pixel (one in each region) — slower
// than midpoint but straightforward to implement and understand.
// ============================================================

#include "ellipse_direct_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                       // All standard headers (sqrt, round, abs)

// Constructor
Ellipse_Direct_DrawingAlgorithm::Ellipse_Direct_DrawingAlgorithm()
    : DrawingAlgorithm("Ellipse_Direct_DrawingAlgorithm") {

}

// ── Helper: 4-fold symmetry ───────────────────────────────────
// An ellipse has 4 mirror axes: horizontal, vertical.
// Given offset (x,y) from center (xc,yc), plot all 4 quadrants.
void Ellipse_Direct_DrawingAlgorithm::Draw4Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc + x, yc + y, color); // Quadrant 1: ( x, y)
    sw->setPixel(xc + x, yc - y, color); // Quadrant 4: ( x,-y) — mirror across x-axis
    sw->setPixel(xc - x, yc + y, color); // Quadrant 2: (-x, y) — mirror across y-axis
    sw->setPixel(xc - x, yc - y, color); // Quadrant 3: (-x,-y) — mirror across both axes
}

// ── Core algorithm ────────────────────────────────────────────
void Ellipse_Direct_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0 = ellipse->points[0]; // Center of the ellipse

    double a = p0.euclideanDistance(ellipse->points[1]); // Horizontal semi-axis (distance from center to x-axis point)
    double b = p0.euclideanDistance(ellipse->points[2]); // Vertical semi-axis   (distance from center to y-axis point)

    if(a < 1.0 || b < 1.0) return; // Both radii must be at least 1 pixel — abort for degenerate ellipse

    sw->activate(); // Acquire Windows DC

    // ── REGION 1: flatter part of the ellipse (slope < 1) ────
    // Step x from 0 forward; compute y from the ellipse equation.
    // Continue while b²x ≤ a²y (the slope condition for Region 1)
    int x = 0;
    double y = b; // Start at the top of the ellipse: x=0, y=b
    while ((b * b * x) <= (a * a * y)) {
        Draw4Points(sw, p0.x, p0.y, x, (int)round(y), ellipse->borderColor); // Plot all 4 symmetric points
        x++;                                                    // Advance x by 1
        y = b * sqrt(1.0 - (double)(x * x) / (a * a));         // Recompute y from ellipse equation: y = b*sqrt(1 - x²/a²)
    }

    // ── REGION 2: steeper part of the ellipse (slope > 1) ────
    // Swap roles: step y; compute x.
    // Continue while a²y < b²x (the slope condition for Region 2)
    int iy = 0;
    double ix = a; // Start at the rightmost point: y=0, x=a
    while ((a * a * iy) < (b * b * ix)) {
        Draw4Points(sw, p0.x, p0.y, (int)round(ix), iy, ellipse->borderColor); // Plot all 4 symmetric points
        iy++;                                                    // Advance y by 1
        ix = a * sqrt(1.0 - (double)(iy * iy) / (b * b));       // Recompute x from ellipse equation: x = a*sqrt(1 - y²/b²)
    }

    sw->deactivate(); // Release Windows DC
}

// ── Public entry point ────────────────────────────────────────
void Ellipse_Direct_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE)  {
        std::cerr << "Ellipse_Direct_DrawingAlgorithm::draw : shape to draw must be Ellipse" << std::endl;
        return;
    }
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(ellipse, sw);
}
