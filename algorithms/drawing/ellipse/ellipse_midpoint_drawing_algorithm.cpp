// ============================================================
// ellipse_midpoint_drawing_algorithm.cpp — Midpoint Ellipse Algorithm
//
// CORE IDEA: Similar to the Midpoint Circle algorithm, but an
// ellipse has two different radii: a (horizontal) and b (vertical).
// The implicit equation is: F(x,y) = b²x² + a²y² - a²b²
//
// Because the ellipse curve changes slope continuously, the
// algorithm is split into TWO REGIONS:
//
//   REGION 1 — where the curve slope < 1 (flatter, near the top/bottom)
//              Step along X; decide each step whether Y also moves.
//              Condition to stay in Region 1: 2*b²*x < 2*a²*y
//
//   REGION 2 — where the curve slope > 1 (steeper, near the left/right sides)
//              Step along Y; decide each step whether X also moves.
//              Continue until y reaches 0.
//
// 4-fold symmetry: unlike a circle's 8-fold, an ellipse only
// has 4 mirror axes (horizontal, vertical). Draw4Points plots
// the point and its 3 reflections.
// ============================================================

#include "ellipse_midpoint_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                         // All standard headers

// Constructor — registers the display name
Ellipse_MidPoint_DrawingAlgorithm::Ellipse_MidPoint_DrawingAlgorithm() : DrawingAlgorithm("Ellipse_MidPoint_DrawingAlgorithm") {

}

// ── Helper: exploit 4-fold symmetry ──────────────────────────
// Given computed offset (x,y) from the center (xc,yc), plot the
// same pixel mirrored into all four quadrants of the ellipse.
void Ellipse_MidPoint_DrawingAlgorithm::Draw4Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc + x, yc + y, color); // Quadrant 1: ( x, y)
    sw->setPixel(xc + x, yc - y, color); // Quadrant 4: ( x,-y) — reflect across horizontal axis
    sw->setPixel(xc - x, yc + y, color); // Quadrant 2: (-x, y) — reflect across vertical axis
    sw->setPixel(xc - x, yc - y, color); // Quadrant 3: (-x,-y) — reflect across both axes
}

// ── Core algorithm ────────────────────────────────────────────
void Ellipse_MidPoint_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0 = ellipse->points[0]; // Center of the ellipse (first click)

    // Compute the two semi-axes from the stored control points
    double a = abs(ellipse->points[1].x - p0.x); // Horizontal radius: distance from center to the point on the x-axis
    double b = abs(ellipse->points[2].y - p0.y); // Vertical radius:   distance from center to the point on the y-axis

    if(a < 1.0 || b < 1.0) return; // Both radii must be at least 1 pixel to be visible

    // Pre-compute squares of the radii — used repeatedly in the decision variable updates
    long long a2 = a * a;        // a²
    long long b2 = b * b;        // b²
    long long twoA2 = 2 * a2;    // 2a² — used as the incremental update for Region 2's dy term
    long long twoB2 = 2 * b2;    // 2b² — used as the incremental update for Region 1's dx term

    int x = 0;   // Start at x=0, y=b (the very top of the ellipse)
    int y = b;

    sw->activate(); // Acquire the Windows drawing context

    // ── REGION 1: slope < 1 ───────────────────────────────────
    // The ellipse is relatively flat here (near the top and bottom arcs).
    // We step x forward and decide at each step whether to also decrease y.

    // Decision variable d1 derived by evaluating F at the midpoint (1, b - 0.5):
    //   d1 = b²*(1)² + a²*(b-0.5)² - a²b²
    //      = b² - a²*b + 0.25*a²
    double d1 = b2 - (a2 * b) + (0.25 * a2);

    long long dx = twoB2 * x; // Partial derivative in x direction (starts at 0, increments by 2b² each x step)
    long long dy = twoA2 * y; // Partial derivative in y direction (starts at 2a²b, decrements by 2a² each y step)

    // Stay in Region 1 while the horizontal force (dx) is less than the vertical force (dy)
    while (dx < dy) {
        Draw4Points(sw, p0.x, p0.y, x, y, ellipse->borderColor); // Plot all 4 symmetric points

        if (d1 < 0) {
            // Midpoint is INSIDE the ellipse → choose the pixel that stays on the same row (step x only)
            x++;           // Advance x by 1
            dx += twoB2;   // Update the dx accumulator: dx = 2b²*(x+1) — just add 2b²
            d1 += dx + b2; // Update decision variable: add dx contribution and a b² correction
        } else {
            // Midpoint is OUTSIDE → choose the pixel that also drops one row (step x AND y)
            x++;           // Advance x
            y--;           // Decrease y by 1 (move downward toward the ellipse's equator)
            dx += twoB2;   // Update dx accumulator
            dy -= twoA2;   // Update dy accumulator (y decreased, so subtract 2a²)
            d1 += dx - dy + b2; // Update decision variable with both x and y contributions
        }
    }

    // ── REGION 2: slope > 1 ───────────────────────────────────
    // The ellipse is steep here (near the left and right sides).
    // We switch: now step Y forward and decide whether X also moves.

    // Decision variable d2 derived by evaluating F at midpoint (x+0.5, y-1):
    //   d2 = b²*(x+0.5)² + a²*(y-1)² - a²b²
    double d2 = (b2 * (x + 0.5) * (x + 0.5)) + (a2 * (y - 1) * (y - 1)) - (a2 * b2);

    // Continue until y reaches 0 (the widest point of the ellipse — the horizontal midline)
    while (y >= 0) {
        Draw4Points(sw, p0.x, p0.y, x, y, ellipse->borderColor); // Plot all 4 symmetric points

        if (d2 > 0) {
            // Midpoint is OUTSIDE the ellipse → stay on the same column (step y only)
            y--;           // Decrease y by 1
            dy -= twoA2;   // Update dy accumulator
            d2 += a2 - dy; // Update decision variable
        } else {
            // Midpoint is INSIDE → also move x outward (step both y AND x)
            y--;           // Decrease y
            x++;           // Increase x (moving outward toward the widest part)
            dx += twoB2;   // Update dx accumulator
            dy -= twoA2;   // Update dy accumulator
            d2 += dx - dy + a2; // Update decision variable with both contributions
        }
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Ellipse_MidPoint_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE) return; // Wrong shape type — abort silently
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(ellipse, sw);
}
