// ============================================================
// rectangle_polygon_clipping_algorithm.cpp — Sutherland-Hodgman Polygon Clipping
//
// CORE IDEA: Clip a polygon so that only the portion inside the
// rectangle is drawn. Unlike Cohen-Sutherland (which clips a LINE
// in one pass), a polygon must be clipped against EACH EDGE of
// the rectangle separately, one at a time.
//
// This is the SUTHERLAND-HODGMAN algorithm:
//   1. Start with the original polygon's vertex list.
//   2. Clip the polygon against the LEFT edge → produces new vertex list.
//   3. Clip that result against the RIGHT edge → produces another list.
//   4. Clip against BOTTOM edge.
//   5. Clip against TOP edge.
//   Result: a new polygon whose vertices all lie inside the rectangle.
//
// HOW EACH EDGE CLIP WORKS (e.g., clip against left edge at x = xmin):
//   Walk the polygon edge by edge (s → e for each consecutive pair).
//   Four cases:
//     s inside, e inside   → output e
//     s inside, e outside  → output intersection
//     s outside, e inside  → output intersection, then output e
//     s outside, e outside → output nothing
//
// After all four clips, draw the outline of the surviving polygon.
// ============================================================

#include "rectangle_polygon_clipping_algorithm.h"              // Header declaring this class
#include "../../../core/shapes/line.h"                          // Line shape — used to draw the clipped polygon's edges
#include "../../drawing/line/line_midpoint_drawing_algorithm.h" // Midpoint line drawer
#include <algorithm>                                            // std::fabs
#include <cmath>                                                // floating-point math
#include <iostream>                                             // std::cerr for error messages

constexpr double kEps = 1e-9; // Tiny epsilon used in comparisons to avoid floating-point precision errors
                                // e.g., a point "on" the boundary might be 1e-15 outside — treat it as inside

// Constructor
Rectangle_Polygon_ClippingAlgorithm::Rectangle_Polygon_ClippingAlgorithm()
    : ClippingAlgorithm("Rectangle_Polygon_ClippingAlgorithm") {}

// ── Intersection helpers ──────────────────────────────────────

// Find where segment a→b crosses a VERTICAL line at x = xLine
// Uses linear interpolation: at t = (xLine-a.x)/(b.x-a.x), y = a.y + t*(b.y-a.y)
Point Rectangle_Polygon_ClippingAlgorithm::intersectVertical(const Point &a, const Point &b, double xLine) const {
    Point res{xLine, a.y};              // Start with x=xLine; y will be updated below
    const double dx = b.x - a.x;       // Horizontal span of segment
    if (std::fabs(dx) > kEps) {         // Avoid divide-by-zero for vertical segments
        const double t = (xLine - a.x) / dx; // Parametric t: how far along the segment we are at x=xLine
        res.y = a.y + t * (b.y - a.y); // Interpolate y at that t value
    }
    return res; // Return the intersection point
}

// Find where segment a→b crosses a HORIZONTAL line at y = yLine
Point Rectangle_Polygon_ClippingAlgorithm::intersectHorizontal(const Point &a, const Point &b, double yLine) const {
    Point res{a.x, yLine};             // Start with y=yLine; x will be updated below
    const double dy = b.y - a.y;      // Vertical span of segment
    if (std::fabs(dy) > kEps) {        // Avoid divide-by-zero for horizontal segments
        const double t = (yLine - a.y) / dy; // Parametric t at y=yLine
        res.x = a.x + t * (b.x - a.x); // Interpolate x at that t value
    }
    return res;
}

// ── One-edge clip functions ───────────────────────────────────
// Each function clips the polygon against ONE side of the rectangle.
// All four follow the same Sutherland-Hodgman pattern.

// Clip against the LEFT edge (keep points where x >= xmin)
std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstLeft(const std::vector<Point> &input, double xmin) const {
    std::vector<Point> output; // Will hold the surviving vertices after this clip
    if (input.empty()) return output;

    Point s = input.back(); // Start with the last vertex as the "previous" vertex (closes the polygon loop)
    for (const Point &e : input) {        // Walk every edge s→e
        const bool sIn = s.x >= xmin - kEps; // Is s on the INSIDE (x >= xmin)?
        const bool eIn = e.x >= xmin - kEps; // Is e on the INSIDE?

        if (eIn) {
            if (!sIn) {
                // s was outside, e is inside → the edge crosses the boundary inward
                // Output the crossing point so the clipped polygon starts exactly at the boundary
                output.push_back(intersectVertical(s, e, xmin));
            }
            output.push_back(e); // e is inside → keep it
        } else if (sIn) {
            // s was inside, e is outside → the edge crosses the boundary outward
            // Output the crossing point so the clipped polygon ends exactly at the boundary
            output.push_back(intersectVertical(s, e, xmin));
            // Do NOT output e — it's outside
        }
        // If both s and e are outside → output nothing for this edge

        s = e; // Advance: e becomes the new "previous" vertex for the next iteration
    }
    return output; // The resulting polygon clipped to the left boundary
}

// Clip against the RIGHT edge (keep points where x <= xmax)
std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstRight(const std::vector<Point> &input, double xmax) const {
    std::vector<Point> output;
    if (input.empty()) return output;

    Point s = input.back(); // Start from the last vertex to close the polygon
    for (const Point &e : input) {
        const bool sIn = s.x <= xmax + kEps; // Inside = x ≤ xmax
        const bool eIn = e.x <= xmax + kEps;
        if (eIn) {
            if (!sIn) output.push_back(intersectVertical(s, e, xmax)); // Crossing point inward
            output.push_back(e);                                        // Keep e
        } else if (sIn) {
            output.push_back(intersectVertical(s, e, xmax));            // Crossing point outward
        }
        s = e;
    }
    return output;
}

// Clip against the TOP edge (keep points where y >= ymin — remember y grows downward on screen)
std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstTop(const std::vector<Point> &input, double ymin) const {
    std::vector<Point> output;
    if (input.empty()) return output;

    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.y >= ymin - kEps; // Inside = y ≥ ymin (below the top boundary in screen coords)
        const bool eIn = e.y >= ymin - kEps;
        if (eIn) {
            if (!sIn) output.push_back(intersectHorizontal(s, e, ymin)); // Crossing point inward
            output.push_back(e);
        } else if (sIn) {
            output.push_back(intersectHorizontal(s, e, ymin));            // Crossing point outward
        }
        s = e;
    }
    return output;
}

// Clip against the BOTTOM edge (keep points where y <= ymax)
std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstBottom(const std::vector<Point> &input, double ymax) const {
    std::vector<Point> output;
    if (input.empty()) return output;

    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.y <= ymax + kEps; // Inside = y ≤ ymax (above the bottom boundary in screen coords)
        const bool eIn = e.y <= ymax + kEps;
        if (eIn) {
            if (!sIn) output.push_back(intersectHorizontal(s, e, ymax)); // Crossing point inward
            output.push_back(e);
        } else if (sIn) {
            output.push_back(intersectHorizontal(s, e, ymax));            // Crossing point outward
        }
        s = e;
    }
    return output;
}

// ── Sutherland-Hodgman: clip against all 4 edges in sequence ──
std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::sutherlandHodgman(
    const std::vector<Point> &subject,
    double xmin, double xmax, double ymin, double ymax) const
{
    std::vector<Point> work = clipAgainstLeft(subject, xmin);  // Pass 1: clip against left boundary
    work = clipAgainstRight(work, xmax);                       // Pass 2: clip the result against right boundary
    work = clipAgainstBottom(work, ymax);                      // Pass 3: clip against bottom boundary
    work = clipAgainstTop(work, ymin);                         // Pass 4: clip against top boundary
    return work; // Final result: polygon vertices that all lie inside the rectangle
}

// ── Draw the clipped polygon's outline ───────────────────────
void Rectangle_Polygon_ClippingAlgorithm::drawPolygonOutline(
    const std::vector<Point> &vertices, COLORREF borderColor, ScreenWriter *sw) const
{
    const int n = static_cast<int>(vertices.size());
    if (n < 2) return; // Nothing to draw for 0 or 1 points

    if (n == 2) {
        // Special case: the clipped polygon collapsed to just a line segment
        Line *line = new Line();
        line->borderColor = borderColor;
        line->addPoint(vertices[0]);
        line->addPoint(vertices[1]);
        DrawingAlgorithm *drawer = new Line_Midpoint_DrawingAlgorithm();
        drawer->draw(*line, sw); // Draw the single line segment
        return;
    }

    // General case: draw each edge of the clipped polygon
    for (int i = 0; i < n; ++i) {
        Line *line = new Line();
        line->borderColor = borderColor;
        line->addPoint(vertices[i]);              // Start of this edge
        line->addPoint(vertices[(i + 1) % n]);    // End of this edge (% n wraps the last edge back to vertex 0)
        DrawingAlgorithm *drawer = new Line_Midpoint_DrawingAlgorithm();
        drawer->draw(*line, sw); // Draw this edge using Bresenham's midpoint algorithm
    }
}

// ── Main algorithm entry ──────────────────────────────────────
void Rectangle_Polygon_ClippingAlgorithm::runAlgorithm(Shape *polygon, RectangleShape *rectangle, ScreenWriter *sw) const {
    if (polygon == nullptr || rectangle == nullptr || sw == nullptr) return; // Null check

    if (polygon->points.size() < 2 || rectangle->points.size() < 2) return; // Need at least edges defined

    // Extract the four boundary values from the rectangle
    const double xmin = rectangle->getTopLeft().x;     // Left boundary
    const double xmax = rectangle->getTopRight().x;    // Right boundary
    const double ymin = rectangle->getTopRight().y;    // Top boundary (smallest y)
    const double ymax = rectangle->getBottomLeft().y;  // Bottom boundary (largest y)

    // Run Sutherland-Hodgman to get the clipped polygon vertices
    const std::vector<Point> clipped = sutherlandHodgman(polygon->getSidePoints(), xmin, xmax, ymin, ymax);

    if (clipped.size() < 2) return; // Entirely clipped away — nothing survives to draw

    drawPolygonOutline(clipped, polygon->borderColor, sw); // Draw the surviving outline
}

// ── Public entry point called by AppManager ───────────────────
void Rectangle_Polygon_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputRegion.getType() != SHAPE_RECTANGLE) {
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }

    Shape *polygon = inputShape.clone();                                           // Clone polygon
    RectangleShape *rectangle = dynamic_cast<RectangleShape *>(inputRegion.clone()); // Clone + cast region

    if (rectangle == nullptr) {
        delete polygon;
        return; // Cast failed — shouldn't happen after the type check above, but guard anyway
    }

    this->runAlgorithm(polygon, rectangle, sw); // Run the full Sutherland-Hodgman clip

    delete polygon;   // Free the cloned shapes — we own them
    delete rectangle;
}
