// ============================================================
// line_midpoint_drawing_algorithm.cpp — Bresenham / Midpoint Line Algorithm
//
// CORE IDEA: Uses only INTEGER arithmetic — no division, no
// floating-point, no round(). This makes it faster than DDA.
//
// HOW IT WORKS (horizontal case):
// At each x step, the "true" line y-value is somewhere between
// two candidate pixel rows. The algorithm asks: is the midpoint
// between those two candidate pixels ABOVE or BELOW the line?
// It answers that question using a "decision variable" d that
// is updated with simple additions every step.
//
//   d < 0  → midpoint is below the line → stay on the same row
//   d ≥ 0  → midpoint is above the line → move up/down one row
//
// d starts at (2*dy - dx) and is updated by either:
//   +2*dy        (if we stayed on the same row)
//   +2*(dy - dx) (if we moved to the next row)
//
// The vertical case mirrors this logic with axes swapped.
// ============================================================

#include "line_midpoint_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                      // All standard headers (abs, etc.)

// Constructor — registers the algorithm's display name with the base class
Line_Midpoint_DrawingAlgorithm::Line_Midpoint_DrawingAlgorithm() : DrawingAlgorithm("Line_Midpoint_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Line_Midpoint_DrawingAlgorithm::runAlgorithm(Line* line, ScreenWriter *sw) const {
    Point p0(line->points[0]); // Start point of the line
    Point p1(line->points[1]); // End point of the line

    // Extract integer pixel coordinates (algorithms work on discrete pixels)
    int x0 = p0.x; // Starting x
    int y0 = p0.y; // Starting y

    int x1 = p1.x; // Ending x
    int y1 = p1.y; // Ending y

    int dx = abs(x1 - x0); // Absolute horizontal span — used to compute the decision variable
    int dy = abs(y1 - y0); // Absolute vertical span   — used to compute the decision variable

    // Determine which direction to step; depends on whether end is right/left of start
    int sx = (x0 < x1) ? 1 : -1; // +1 step right if x1 > x0, -1 step left otherwise
    int sy = (y0 < y1) ? 1 : -1; // +1 step down  if y1 > y0, -1 step up  otherwise

    int x = x0; // Current x position — starts at the beginning of the line
    int y = y0; // Current y position — starts at the beginning of the line

    sw->activate(); // Acquire the Windows drawing context before writing any pixels

    sw->setPixel(x, y, line->borderColor); // Draw the first pixel at the start point

    // ── Case 1: line is more horizontal (dx ≥ dy) ────────────
    // Step one pixel at a time along X; decide each step whether Y also moves
    if(dx >= dy) {
        int d = 2 * dy - dx; // Initial decision variable
                              // Derived from substituting the midpoint into the implicit line equation:
                              // F(x,y) = dy*x - dx*y + c. At the first midpoint, this simplifies to 2*dy - dx.

        while(x != x1) {     // Loop until we reach the last x pixel
            if(d >= 0) {
                // Midpoint is ABOVE the true line → the pixel in the next row is closer to the line
                y += sy;              // Move y one step (up or down depending on direction)
                d += 2 * (dy - dx);   // Update d: subtract dx contribution, add dy contribution
            } else {
                // Midpoint is BELOW the true line → stay on the same y row
                d += 2 * dy;          // Update d: only add dy contribution
            }
            x += sx;                  // Always advance one x step toward x1
            sw->setPixel(x, y, line->borderColor); // Plot the chosen pixel
        }
    }
    // ── Case 2: line is more vertical (dy > dx) ──────────────
    // Mirror of Case 1: step along Y; decide whether X also moves each step
    else {
        int d = 2 * dx - dy; // Initial decision variable for the vertical-dominant case
                              // Same derivation as above but with dx and dy roles swapped

        while(y != y1) {     // Loop until we reach the last y pixel
            if(d >= 0) {
                // Midpoint logic: the adjacent x column is closer → move x
                x += sx;              // Move x one step
                d += 2 * (dx - dy);   // Update d
            } else {
                d += 2 * dx;          // Stay on same x column; update d
            }
            y += sy;                  // Always advance one y step toward y1
            sw->setPixel(x, y, line->borderColor); // Plot the chosen pixel
        }
    }

    sw->deactivate(); // Release the Windows drawing context — must pair with activate()
}

// ── Public entry point called by AppManager ───────────────────
void Line_Midpoint_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    // Type-safety check: only proceed if the shape is actually a Line
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Line_Midpoint_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone()); // Clone so the original shape is not modified
                                                           // dynamic_cast safely converts Shape* → Line*
    this->runAlgorithm(line, sw); // Run the Bresenham algorithm on the typed pointer
}
