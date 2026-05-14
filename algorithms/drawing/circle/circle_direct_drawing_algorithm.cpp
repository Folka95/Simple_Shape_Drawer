// ============================================================
// circle_direct_drawing_algorithm.cpp — Direct Circle Algorithm
//
// CORE IDEA: Uses the circle equation x² + y² = r² directly.
// Starting from the top of the circle (x=0, y=r), increment x
// by 1 each step and solve for y exactly:
//   y = sqrt(r² - x²)
//
// Because a circle has 8-fold symmetry (symmetric in all four
// quadrants AND across the diagonals), we only need to compute
// 1/8 of the circle and mirror the result to 7 other positions.
// That's what Draw8Points does.
//
// Disadvantage vs Midpoint: calls sqrt() every step — more
// expensive. The Midpoint algorithm avoids sqrt entirely.
// ============================================================

#include "circle_direct_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                      // All standard headers (sqrt, pow, round, abs)

// Constructor — registers the display name with the base DrawingAlgorithm class
Circle_Direct_DrawingAlgorithm::Circle_Direct_DrawingAlgorithm() : DrawingAlgorithm("Circle_Direct_DrawingAlgorithm") {

}

// ── Helper: exploit 8-fold symmetry ──────────────────────────
// Given a computed (x, y) offset in the first octant, this
// function plots the matching pixel in all 8 symmetric positions
// around the center (xc, yc).
void Circle_Direct_DrawingAlgorithm::Draw8Points(ScreenWriter *sw, int xc, int yc, int x, int y, COLORREF color) const {
    sw->setPixel(xc+x, yc+y, color); // Quadrant 1, lower octant  ( x, y)
    sw->setPixel(xc+x, yc-y, color); // Quadrant 1, upper octant  ( x,-y) — reflect across x-axis
    sw->setPixel(xc-x, yc+y, color); // Quadrant 2, lower octant  (-x, y) — reflect across y-axis
    sw->setPixel(xc-x, yc-y, color); // Quadrant 3               (-x,-y) — reflect across both axes
    sw->setPixel(xc+y, yc+x, color); // Quadrant 1, reflected across y=x diagonal ( y, x)
    sw->setPixel(xc+y, yc-x, color); // Quadrant 4, diagonal reflection           ( y,-x)
    sw->setPixel(xc-y, yc+x, color); // Quadrant 2, diagonal reflection           (-y, x)
    sw->setPixel(xc-y, yc-x, color); // Quadrant 3, diagonal reflection           (-y,-x)
    // Together these 8 points cover the full circle boundary symmetrically
}

// ── Core algorithm ────────────────────────────────────────────
void Circle_Direct_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]); // Center of the circle (user's first click)
    Point p1(circle->points[1]); // A point on the edge (user's second click — defines radius)

    // Compute radius using Euclidean distance between center and edge point
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));

    if(radius < 1.0) {
        // A radius less than 1 pixel is invisible — log and abort rather than produce garbage output
        std::cerr << "Circle_Direct_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }

    int x = 0;         // Start at the top of the first octant: x = 0
    int y = radius;    // y starts at the full radius (topmost point)
    int Rsq = radius * radius; // Pre-compute r² once so we don't recalculate it inside the loop

    sw->activate(); // Acquire the Windows drawing context

    Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor); // Draw the initial 8 points at (0, r)

    // Walk from x=0 toward x=y (the 45° diagonal — where the first octant ends)
    while(x < y) {
        x++;                                              // Move one pixel to the right along the x axis
        y = round(sqrt((double)(Rsq - x*x)));            // Recompute y from the circle equation: y = sqrt(r² - x²)
                                                          // round() maps the exact float y to the nearest pixel row
        Draw8Points(sw, circle->points[0].x, circle->points[0].y, x, y, circle->borderColor); // Mirror to all 8 octants
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Circle_Direct_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        // Wrong shape type — print an error and abort rather than crash on the cast below
        std::cerr << "Circle_Direct_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone()); // Clone so the original is not modified; cast to Circle*
    this->runAlgorithm(circle, sw); // Run the direct algorithm
}
