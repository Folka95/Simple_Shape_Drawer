// ============================================================
// circle_quarter_with_line_filling_algorithm.cpp
//
// CORE IDEA: Fill ONE QUARTER of a circle using HORIZONTAL LINE
// SEGMENTS instead of concentric arcs (contrast with the
// "with_circle" variant that uses arcs).
//
// The algorithm uses the MIDPOINT CIRCLE approach to iterate
// through the boundary pixels of the full circle (x from 0 to r,
// y from r to 0). For each boundary point (cx+x, cy+y), it draws
// a horizontal line from the center to that boundary point,
// but only in the selected quadrant.
//
// QUADRANT to LINE MAPPING:
//   Q1 (right-down): horizontal lines from center.x to (center.x + x/y)
//   Q2 (left-down):  horizontal lines from (center.x - x/y) to center.x
//   Q3 (left-up):    same as Q2 but negative y rows
//   Q4 (right-up):   same as Q1 but negative y rows
//
// The result is the same quarter-fill but with a scanline-style
// pattern of horizontal strokes rather than concentric arcs.
// ============================================================

#include "circle_quarter_with_line_filling_algorithm.h" // Header declaring this class

// Constructor
Circle_QuarterWithLine_FillingAlgorithm::Circle_QuarterWithLine_FillingAlgorithm()
    : FillingAlgorithm("Circle_QuarterWithLine_FillingAlgorithm") {

}

// ── Draw one horizontal fill line using Bresenham ────────────
// Draws a horizontal-ish line from (x1,y1) to (x2,y2) using the
// midpoint algorithm — respects the clipping region.
void Circle_QuarterWithLine_FillingAlgorithm::drawMidpointLine(int x1, int y1, int x2, int y2, ScreenWriter *sw, COLORREF color, const Shape &clippingRegion) const {
    int dx = abs(x2 - x1); // Absolute horizontal span
    int dy = abs(y2 - y1); // Absolute vertical span
    int sx = (x1 < x2) ? 1 : -1; // Step direction in x (+1 right, -1 left)
    int sy = (y1 < y2) ? 1 : -1; // Step direction in y (+1 down, -1 up)

    int x = x1; // Current x position
    int y = y1; // Current y position

    // Draw the first pixel (check clipping)
    if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
        sw->setPixel(x, y, color);
    }

    // ── Case 1: more horizontal ───────────────────────────────
    if (dx >= dy) {
        int d = 2 * dy - dx; // Initial decision variable (same as line midpoint algorithm)
        while (x != x2) {
            if (d >= 0) {
                y += sy;          // Move y by one step (midpoint chose the adjacent row)
                d += 2 * (dy - dx); // Update decision variable for SE
            } else {
                d += 2 * dy;      // Update decision variable for E
            }
            x += sx; // Always step x
            if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
                sw->setPixel(x, y, color); // Only draw if inside clipping region
            }
        }
    }
    // ── Case 2: more vertical ─────────────────────────────────
    else {
        int d = 2 * dx - dy; // Initial decision variable (y-dominant case)
        while (y != y2) {
            if (d >= 0) {
                x += sx;          // Step x
                d += 2 * (dx - dy);
            } else {
                d += 2 * dx;
            }
            y += sy; // Always step y
            if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
                sw->setPixel(x, y, color);
            }
        }
    }
}

// ── Fill the selected quarter using horizontal lines ──────────
void Circle_QuarterWithLine_FillingAlgorithm::fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    int q = circle.scanQuarter(startPoint); // Determine which quarter the user clicked

    Point center  = circle.getCenter();  // Center of the circle
    double radius = circle.getRadius();  // Radius of the circle

    if (q == 0 || q == 5) {
        std::cerr << "Circle_QuarterWithLine_FillingAlgorithm::fill_helper : Please Choose a valid quarter" << std::endl;
        return;
    }

    // Iterate through the first octant using Midpoint circle variables
    int x = 0;
    int y = (int)radius; // Start at the top of the first octant
    int d = 1 - (int)radius; // Initial decision variable (same as midpoint circle)

    while (x <= y) {
        // For each (x, y) boundary point, draw fill lines based on quadrant
        // Two lines per step because the circle boundary gives two distinct y-rows per x in the first octant
        if (q == 1) { // Bottom-right quadrant
            drawMidpointLine(center.x, center.y + y, center.x + x, center.y + y, sw, circle.fillColor, clippingRegion); // Fill row at y+Y from center.x to center.x+x
            drawMidpointLine(center.x, center.y + x, center.x + y, center.y + x, sw, circle.fillColor, clippingRegion); // Fill row at y+x from center.x to center.x+y (diagonal reflection)
        } else if (q == 2) { // Bottom-left quadrant
            drawMidpointLine(center.x, center.y + y, center.x - x, center.y + y, sw, circle.fillColor, clippingRegion);
            drawMidpointLine(center.x, center.y + x, center.x - y, center.y + x, sw, circle.fillColor, clippingRegion);
        } else if (q == 3) { // Top-left quadrant
            drawMidpointLine(center.x, center.y - y, center.x - x, center.y - y, sw, circle.fillColor, clippingRegion);
            drawMidpointLine(center.x, center.y - x, center.x - y, center.y - x, sw, circle.fillColor, clippingRegion);
        } else if (q == 4) { // Top-right quadrant
            drawMidpointLine(center.x, center.y - y, center.x + x, center.y - y, sw, circle.fillColor, clippingRegion);
            drawMidpointLine(center.x, center.y - x, center.x + y, center.y - x, sw, circle.fillColor, clippingRegion);
        }

        // Advance to the next boundary point using the Midpoint decision variable
        if (d < 0) {
            d += 2 * x + 3; // E choice: only x moves, update d
        } else {
            d += 2 * (x - y) + 5; // SE choice: both x and y move, update d
            y--;            // Decrease y (move boundary inward)
        }
        x++; // Always advance x
    }
}

// ── Public entry point ────────────────────────────────────────
void Circle_QuarterWithLine_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_QuarterWithLine_FillingAlgorithm::fill : Region must be a Circle" << std::endl;
        return;
    }
    const Circle* circle = dynamic_cast<const Circle*>(&shape); // Cast to Circle* to access circle methods
    if (!circle) return;

    sw->activate(); // Acquire Windows DC
    this->fill_helper(*circle, clippingRegion, startPoint, sw);
    sw->deactivate(); // Release Windows DC
}
