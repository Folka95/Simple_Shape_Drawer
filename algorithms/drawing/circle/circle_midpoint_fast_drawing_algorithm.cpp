// ============================================================
// circle_midpoint_fast_drawing_algorithm.cpp — Optimized Midpoint Circle
//
// CORE IDEA: Same Bresenham/Midpoint circle algorithm as
// circle_midpoint_drawing_algorithm.cpp, but the decision
// variable update formula is further simplified using two
// pre-computed constants c1 and c2 to avoid recomputing
// (2x + 2) and (2x - 2y + 5) every step.
//
// Instead of:
//   d < 0  →  d += 2x + 2
//   d >= 0 →  d += 2x - 2y + 5, y--
//
// This version uses:
//   c1 = 3  (updated: c1 += 2 each step, so c1 = 2x+3 at step x)
//   c2 = 5 - 2r  (updated differently when y changes)
//
//   d < 0  →  d += c1,       c2 += 2
//   d >= 0 →  d += c2,  c2 += 4,  y--
//   Always:  c1 += 2, x++
//
// c1 is always 2x+3 (equivalent to 2*(x+1)+1 in the standard form).
// c2 absorbs the y-decrement terms.
// The result is identical output with marginally fewer operations per step.
// ============================================================

#include "circle_midpoint_fast_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                             // All standard headers (sqrt, pow)

// Constructor
Circle_MidPoint_Fast_DrawingAlgorithm::Circle_MidPoint_Fast_DrawingAlgorithm()
    : DrawingAlgorithm("Circle_MidPoint_Fast_DrawingAlgorithm") {

}

// ── Helper: 8-fold symmetry ───────────────────────────────────
// Same as the other circle algorithms — plots one (x,y) offset at all 8 mirror positions
void Circle_MidPoint_Fast_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color); // ( x, y)
    sw->setPixel(xc+x, yc-y, color); // ( x,-y)
    sw->setPixel(xc-x, yc+y, color); // (-x, y)
    sw->setPixel(xc-x, yc-y, color); // (-x,-y)
    sw->setPixel(xc+y, yc+x, color); // ( y, x) — diagonal reflection
    sw->setPixel(xc+y, yc-x, color); // ( y,-x)
    sw->setPixel(xc-y, yc+x, color); // (-y, x)
    sw->setPixel(xc-y, yc-x, color); // (-y,-x)
}

// ── Core optimized algorithm ──────────────────────────────────
void Circle_MidPoint_Fast_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]); // Center
    Point p1(circle->points[1]); // Edge point

    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2)); // Compute radius from Euclidean distance

    if(radius < 1.0) {
        std::cerr << "Circle_MidPoint_Fast_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }

    int x = 0;        // Start at x = 0 (top of first octant)
    int y = radius;   // Start at y = r (topmost point)

    int d  = 1 - radius;       // Initial decision variable: d = 1 - r (same as standard midpoint)
    int c1 = 3;                // c1 = 2*0 + 3 = 3  (the constant added to d when moving E only)
                               // Updated as c1 += 2 each step: c1 is always 2x+3
    int c2 = 5 - 2 * radius;   // c2 encodes the SE move cost. Starts at 5 - 2r.
                               // Updated as c2 += 2 when E chosen, c2 += 4 when SE chosen

    sw->activate(); // Acquire Windows DC

    Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor); // Initial 8 points

    while(x < y) { // Iterate through the first octant
        if(d < 0) {
            // E choice: midpoint is inside → stay on same row (only x increases)
            d  += c1;  // Update d with the "East" increment  = 2x + 3
            c2 += 2;   // When only x changes, c2 grows by 2 (y didn't change, so lose one y term)
        }
        else {
            // SE choice: midpoint is outside → drop one row (both x and y change)
            d  += c2;  // Update d with the "South-East" increment
            c2 += 4;   // When both x and y change, c2 grows by 4 (extra y contribution)
            y--;       // Move y inward by one
        }
        c1 += 2; // c1 = 2x+3 advances by 2 every step as x increments
        x++;     // Always advance x by 1
        Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor);
    }

    sw->deactivate(); // Release Windows DC
}

// ── Public entry point ────────────────────────────────────────
void Circle_MidPoint_Fast_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_MidPoint_Fast_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone()); // Clone + cast
    this->runAlgorithm(circle, sw);
}
