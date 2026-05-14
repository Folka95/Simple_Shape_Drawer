// ============================================================
// rectangle_line_clipping_algorithm.cpp — Cohen-Sutherland Line Clipping
//
// CORE IDEA: Given a line segment and a rectangular clipping window,
// determine the portion of the line that lies INSIDE the rectangle
// and draw only that portion.
//
// The Cohen-Sutherland algorithm works using "outcodes" — a 4-bit
// code assigned to each endpoint that says which side(s) of the
// rectangle boundary the point is OUTSIDE of:
//
//   Bit 0 (left):   x < xleft
//   Bit 1 (top):    y < ytop
//   Bit 2 (right):  x > xright
//   Bit 3 (bottom): y > ybottom
//
// THREE CASES:
//   1. Both outcodes = 0000 → entire line is INSIDE → draw it.
//   2. (outcode1 AND outcode2) ≠ 0 → both endpoints are on the SAME
//      outside side → entire line is OUTSIDE → discard.
//   3. Otherwise → the line crosses at least one boundary →
//      clip: move the outside endpoint to the boundary intersection,
//      recompute its outcode, and repeat until case 1 or 2 applies.
// ============================================================

#include "rectangle_line_clipping_algorithm.h"          // Header declaring this class
#include "../../../core/shapes/line.h"                   // Line shape — needed to create a clipped line for drawing
#include "../../drawing/line/line_midpoint_drawing_algorithm.h" // Used to draw the clipped line segment

// ── Outcode: stores which sides of the rectangle a point is outside of ──
union OutCode {
    unsigned All: 4;       // All 4 bits as one value — used for quick "is any bit set?" checks
    struct {
        unsigned left:   1; // Bit 0: 1 if point is to the left   of the clipping window
        unsigned top:    1; // Bit 1: 1 if point is above          the clipping window (y < ytop)
        unsigned right:  1; // Bit 2: 1 if point is to the right  of the clipping window
        unsigned bottom: 1; // Bit 3: 1 if point is below          the clipping window (y > ybottom)
    };
};

// Compute the 4-bit outcode for point (x,y) relative to the rectangle defined by the four boundaries
OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0; // Start with all bits clear (assume point is inside)

    // Set the appropriate bits for each side the point is outside of
    if      (x < xleft)   out.left   = 1; // Point is to the left of the window
    else if (x > xright)  out.right  = 1; // Point is to the right of the window
    if      (y < ytop)    out.top    = 1; // Point is above the window
    else if (y > ybottom) out.bottom = 1; // Point is below the window

    return out;
}

// ── Compute intersection of the line with a VERTICAL boundary ──
// Given line from (xs,ys) to (xe,ye), find where it crosses x = xBoundary
void Rectangle_Line_ClippingAlgorithm::VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const {
    *xi = x;                                             // x is the boundary value (exact)
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);        // Solve for y using linear interpolation: y = ys + t*(ye-ys) where t = (x-xs)/(xe-xs)
}

// ── Compute intersection of the line with a HORIZONTAL boundary ──
// Given line from (xs,ys) to (xe,ye), find where it crosses y = yBoundary
void Rectangle_Line_ClippingAlgorithm::HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const {
    *yi = y;                                             // y is the boundary value (exact)
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);        // Solve for x using linear interpolation: x = xs + t*(xe-xs) where t = (y-ys)/(ye-ys)
}

// ── Helper: draw the surviving line segment ───────────────────
void Rectangle_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, ScreenWriter *sw) const {
    Line *line = new Line();  // Create a new Line shape to hold the two clipped endpoints
    line->addPoint(p1);       // Set the first endpoint of the clipped segment
    line->addPoint(p2);       // Set the second endpoint of the clipped segment
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm(); // Use Bresenham to draw the segment
    line_midpoint_drawing_algorithm->draw(*line, sw); // Actually draw it to the screen
}

// Constructor
Rectangle_Line_ClippingAlgorithm::Rectangle_Line_ClippingAlgorithm()
    : ClippingAlgorithm("Rectangle_Line_ClippingAlgorithm") {}

// ── Core Cohen-Sutherland algorithm ──────────────────────────
void Rectangle_Line_ClippingAlgorithm::runAlgorithm(Line* line, RectangleShape* rectangle, ScreenWriter *sw) const {
    // Extract the four clipping boundaries from the rectangle
    int xleft   = rectangle->getTopLeft().x;     // Left boundary (minimum x)
    int ytop    = rectangle->getTopLeft().y;      // Top boundary  (minimum y)
    int xright  = rectangle->getBottomRight().x; // Right boundary (maximum x)
    int ybottom = rectangle->getBottomRight().y; // Bottom boundary (maximum y)

    // Extract the current line endpoints (mutable — they'll be adjusted as we clip)
    double x1 = line->points[0].x, y1 = line->points[0].y; // First endpoint
    double x2 = line->points[1].x, y2 = line->points[1].y; // Second endpoint

    // Compute initial outcodes for both endpoints
    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom); // Tells us which sides point 1 is outside of
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom); // Tells us which sides point 2 is outside of

    // ── Clip loop ─────────────────────────────────────────────
    // Continue as long as at least one endpoint is outside AND they're not both on the same outside side
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi; // Will hold the computed intersection point

        if (out1.All) {
            // Point 1 is outside → clip point 1 to the boundary it violates
            if      (out1.left)   VIntersect(x1, y1, x2, y2, xleft,   &xi, &yi); // Clip to left edge
            else if (out1.top)    HIntersect(x1, y1, x2, y2, ytop,    &xi, &yi); // Clip to top edge
            else if (out1.right)  VIntersect(x1, y1, x2, y2, xright,  &xi, &yi); // Clip to right edge
            else                  HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi); // Clip to bottom edge

            x1 = xi; // Move point 1 to the intersection
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom); // Recompute outcode for new point 1
        } else {
            // Point 2 is outside → clip point 2 to the boundary it violates
            if      (out2.left)   VIntersect(x1, y1, x2, y2, xleft,   &xi, &yi); // Clip to left edge
            else if (out2.top)    HIntersect(x1, y1, x2, y2, ytop,    &xi, &yi); // Clip to top edge
            else if (out2.right)  VIntersect(x1, y1, x2, y2, xright,  &xi, &yi); // Clip to right edge
            else                  HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi); // Clip to bottom edge

            x2 = xi; // Move point 2 to the intersection
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom); // Recompute outcode for new point 2
        }
    }

    // ── Final decision ────────────────────────────────────────
    if (!out1.All && !out2.All) {
        // Both outcodes are now 0000 → both endpoints are inside → draw the clipped segment
        Point p1(x1, y1);
        Point p2(x2, y2);
        drawLine(p1, p2, sw);
    }
    // If the condition is false, (out1 & out2) != 0 → both endpoints are on the same outside side → discard the line entirely
}

// ── Public entry point called by AppManager ───────────────────
void Rectangle_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Rectangle_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_RECTANGLE) {
        std::cerr << "Rectangle_Line_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());                         // Clone + cast to Line*
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputRegion.clone()); // Clone + cast to RectangleShape*
    this->runAlgorithm(line, rectangle, sw);
}
