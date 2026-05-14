// ============================================================
// square_line_clipping_algorithm.cpp — Line Clipping Against a Square
//
// CORE IDEA: Identical to rectangle_line_clipping — uses the
// Cohen-Sutherland algorithm, but the clipping boundary is
// extracted from a Square shape rather than a Rectangle.
//
// A square is just a rectangle with equal sides, so the
// same 4-boundary outcode logic applies:
//   left/right: x < xleft or x > xright
//   top/bottom: y < ytop  or y > ybottom
//
// The square's corners are computed from its two stored points,
// and then the exact same clip loop runs as in the rectangle version.
// See rectangle_line_clipping_algorithm.cpp for the full explanation.
// ============================================================

#include "square_line_clipping_algorithm.h"                         // Header declaring this class
#include "../../../core/shapes/line.h"                               // Line shape — for drawing the clipped segment
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"      // Midpoint line drawer
#include <algorithm>                                                 // std::min, std::max

// Constructor
Square_Line_ClippingAlgorithm::Square_Line_ClippingAlgorithm()
    : ClippingAlgorithm("Square_Line_ClippingAlgorithm") {}

// ── Outcode: 4-bit struct saying which sides of the square a point is outside ──
union Square_Line_ClippingAlgorithm::OutCode {
    unsigned All: 4;       // All 4 bits as one value — for quick "any bit set?" check
    struct {
        unsigned left:   1; // 1 if x < xleft
        unsigned top:    1; // 1 if y < ytop
        unsigned right:  1; // 1 if x > xright
        unsigned bottom: 1; // 1 if y > ybottom
    };
};

// Compute which sides of the square a point (x,y) is outside of
Square_Line_ClippingAlgorithm::OutCode Square_Line_ClippingAlgorithm::GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) const {
    OutCode out;
    out.All = 0; // Start with all bits clear
    if      (x < xleft)   out.left   = 1; // Left of window
    else if (x > xright)  out.right  = 1; // Right of window
    if      (y < ytop)    out.top    = 1; // Above window
    else if (y > ybottom) out.bottom = 1; // Below window
    return out;
}

// Find where line (xs,ys)→(xe,ye) crosses vertical boundary x
void Square_Line_ClippingAlgorithm::VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs); // Linear interpolation for y at x
}

// Find where line (xs,ys)→(xe,ye) crosses horizontal boundary y
void Square_Line_ClippingAlgorithm::HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys); // Linear interpolation for x at y
}

// Draw the surviving clipped line segment
void Square_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, COLORREF borderColor, ScreenWriter *sw) const {
    Line *line = new Line();
    line->borderColor = borderColor; // Apply the shape's border color to the drawn segment
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}

// ── Core Cohen-Sutherland algorithm against square boundaries ──
void Square_Line_ClippingAlgorithm::runAlgorithm(Line* line, Square* square, ScreenWriter *sw) const {
    if (line == nullptr || square == nullptr || line->points.size() < 2) return;

    // Extract square boundaries (handle the case where corner order may vary)
    const int xleft   = (std::min)(square->getBottomLeft().x,  square->getBottomRight().x); // Min x
    const int xright  = (std::max)(square->getBottomLeft().x,  square->getBottomRight().x); // Max x
    const int ymin    = (std::min)(square->getTopLeft().y,     square->getBottomRight().y);  // Min y (top)
    const int ymax    = (std::max)(square->getTopLeft().y,     square->getBottomRight().y);  // Max y (bottom)

    double x1 = line->points[0].x, y1 = line->points[0].y; // Line start
    double x2 = line->points[1].x, y2 = line->points[1].y; // Line end

    OutCode out1 = GetOutCode(x1, y1, xleft, ymin, xright, ymax); // Outcode for start point
    OutCode out2 = GetOutCode(x2, y2, xleft, ymin, xright, ymax); // Outcode for end point

    // Cohen-Sutherland clip loop (see rectangle version for full explanation)
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi;
        if (out1.All) {
            // Clip the outside endpoint (out1) to whichever boundary it violates
            if      (out1.left)   VIntersect(x1, y1, x2, y2, xleft,  &xi, &yi);
            else if (out1.top)    HIntersect(x1, y1, x2, y2, ymin,   &xi, &yi);
            else if (out1.right)  VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else                  HIntersect(x1, y1, x2, y2, ymax,   &xi, &yi);
            x1 = xi; y1 = yi; // Move start point to the intersection
            out1 = GetOutCode(x1, y1, xleft, ymin, xright, ymax); // Recompute outcode
        } else {
            if      (out2.left)   VIntersect(x1, y1, x2, y2, xleft,  &xi, &yi);
            else if (out2.top)    HIntersect(x1, y1, x2, y2, ymin,   &xi, &yi);
            else if (out2.right)  VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else                  HIntersect(x1, y1, x2, y2, ymax,   &xi, &yi);
            x2 = xi; y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ymin, xright, ymax);
        }
    }

    if (!out1.All && !out2.All) {
        // Both endpoints now inside → draw the clipped segment
        drawLine(Point(x1, y1), Point(x2, y2), line->borderColor, sw);
    }
    // If (out1 & out2) != 0 → both on same outside side → discard entirely
}

// ── Public entry point ────────────────────────────────────────
void Square_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }
    Line*   line   = dynamic_cast<Line*>  (inputShape.clone());  // Clone + cast to Line*
    Square* square = dynamic_cast<Square*>(inputRegion.clone()); // Clone + cast to Square*
    this->runAlgorithm(line, square, sw);
}
