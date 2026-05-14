// ============================================================
// circle_midpoint_drawing_algorithm.cpp — Midpoint Circle Algorithm
//                                         (also called Bresenham's Circle)
//
// CORE IDEA: Avoids sqrt() entirely by using a decision variable
// d that tracks whether the true circle boundary is above or
// below the midpoint between two candidate pixels.
//
// Starting at (x=0, y=r) and moving clockwise through the
// first octant (x from 0 to r/√2), at each step we must
// choose between two candidate next pixels:
//   • E  = (x+1, y)     — stay on the same row
//   • SE = (x+1, y-1)   — drop down one row
//
// We evaluate the implicit circle function F(x,y) = x²+y²-r²
// at the MIDPOINT between E and SE, i.e. at (x+1, y-0.5):
//   d = F(x+1, y-0.5) = (x+1)² + (y-0.5)² - r²
//
//   d < 0 → midpoint is inside the circle → E is closer → choose E
//   d ≥ 0 → midpoint is outside or on the circle → SE is closer → choose SE
//
// d is updated incrementally (only additions, no sqrt):
//   • If chose E:  d_new = d + 2x + 2       (x++ only)
//   • If chose SE: d_new = d + 2x - 2y + 5  (x++ and y--)
//
// Initial value: d = 1 - r  (derived by evaluating F at the first midpoint)
//
// 8-fold symmetry is exploited exactly as in the Direct algorithm.
// ============================================================

#include "circle_midpoint_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                        // All standard headers

// Constructor — registers the display name
Circle_MidPoint_DrawingAlgorithm::Circle_MidPoint_DrawingAlgorithm() : DrawingAlgorithm("Circle_MidPoint_DrawingAlgorithm") {

}

// ── Helper: exploit 8-fold symmetry ──────────────────────────
// Plots one computed (x,y) offset at all 8 symmetric positions around center (xc, yc)
void Circle_MidPoint_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color); // ( x, y) — first octant
    sw->setPixel(xc+x, yc-y, color); // ( x,-y) — reflect across horizontal axis
    sw->setPixel(xc-x, yc+y, color); // (-x, y) — reflect across vertical axis
    sw->setPixel(xc-x, yc-y, color); // (-x,-y) — reflect across both axes (opposite quadrant)
    sw->setPixel(xc+y, yc+x, color); // ( y, x) — reflect across the y=x diagonal
    sw->setPixel(xc+y, yc-x, color); // ( y,-x) — reflect across diagonal + horizontal
    sw->setPixel(xc-y, yc+x, color); // (-y, x) — reflect across diagonal + vertical
    sw->setPixel(xc-y, yc-x, color); // (-y,-x) — reflect across all three axes of symmetry
}

// ── Core algorithm ────────────────────────────────────────────
void Circle_MidPoint_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]); // Center of the circle
    Point p1(circle->points[1]); // Edge point — used to compute the radius

    // Compute radius as the Euclidean distance from center to edge point
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));

    if(radius < 1.0) {
        std::cerr << "Circle_MidPoint_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }

    int x = 0;        // Start at x=0 (top of the first octant)
    int y = radius;   // Start at y=r (the topmost point)
    int d = 1 - radius; // Initial decision variable: d = 1 - r
                         // Derived by evaluating F(1, r-0.5) = 1 + (r-0.5)² - r² ≈ 1 - r for integer r

    sw->activate(); // Acquire the Windows drawing context

    Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor); // Plot the starting 8 points

    // Iterate through the first octant: x from 0 to y (i.e., until the 45° diagonal)
    while(x < y) {
        if(d < 0) {
            // Midpoint is INSIDE the circle → the pixel staying on the same row (E) is closer
            // Update d for the E choice: d += 2x + 2  (equivalent to d += 2*(x+1))
            d += 2*x + 2; // Only x increments, so only the x² term in F changes
        }
        else {
            // Midpoint is OUTSIDE or ON the circle → the pixel dropping down (SE) is closer
            // Update d for the SE choice: d += 2*(x - y) + 5
            d += 2*(x - y) + 5; // Both x++ and y-- change F, giving this combined update
            y--;                 // Move y inward (toward center) by one pixel
        }
        x++;            // Always move x outward by one pixel (the primary stepping direction)
        Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor); // Plot the new 8 symmetric points
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Circle_MidPoint_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_MidPoint_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone()); // Clone and cast to Circle* for typed access
    this->runAlgorithm(circle, sw);
}
