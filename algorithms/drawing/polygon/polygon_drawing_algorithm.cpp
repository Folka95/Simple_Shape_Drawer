// ============================================================
// polygon_drawing_algorithm.cpp — Polygon Drawing Algorithm
//
// CORE IDEA: A polygon is drawn by connecting its vertices with
// straight line segments. This algorithm:
//   1. Gets the ordered list of vertices from the shape (getSidePoints)
//   2. For each consecutive pair of vertices, draws a line
//   3. The last vertex connects back to the first (% size wraps the index)
//
// All edges are drawn using the Midpoint line algorithm internally.
//
// This is also the default drawing algorithm used at startup
// (before the user selects a specific shape/algorithm), because
// it's the most general — it works for any polygon with N points.
// ============================================================

#include "polygon_drawing_algorithm.h"                     // Header declaring this class
#include "../line/line_midpoint_drawing_algorithm.h"        // Line drawing used for each polygon edge
#include "../../../core/shapes/line.h"                      // Line shape — used as a temporary container per edge
#include <algorithm>                                        // std::sort (included but not directly used here)

// Constructor
Polygon_DrawingAlgorithm::Polygon_DrawingAlgorithm() : DrawingAlgorithm("Polygon_DrawingAlgorithm") {}

// ── Helper: draw a single edge (line segment) ─────────────────
void Polygon_DrawingAlgorithm::drawLine(const Point& p1, const Point& p2, COLORREF color, ScreenWriter *sw) const {
    Line* line = new Line();    // Create a temporary Line shape to hold this edge's two endpoints
    line->addPoint(p1);         // First vertex of this edge
    line->addPoint(p2);         // Second vertex of this edge
    line->borderColor = color;  // Apply the polygon's border color to this edge
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm(); // Use Bresenham to draw the segment
    line_midpoint_drawing_algorithm->draw(*line, sw); // Draw the edge to the screen
}

// ── Core algorithm ────────────────────────────────────────────
void Polygon_DrawingAlgorithm::runAlgorithm(Shape* polygon, ScreenWriter *sw) const {
    vector< Point > points = polygon->getSidePoints(); // Get the ordered vertex list from the shape
                                                        // (may differ from raw points for shapes like rectangles)

    // Draw each edge: from points[i] to points[i+1]
    // The modulo wraps the last edge back to points[0], closing the polygon
    for(int i = 0; i < polygon->points.size(); i++) {
        drawLine(points[i], points[(i + 1) % points.size()], polygon->borderColor, sw);
        //        ^^^^^^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        //        Start      End (wraps last→first to close the shape)
    }
}

// ── Public entry point called by AppManager ───────────────────
void Polygon_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Polygon_DrawingAlgorithm::draw : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputShape.getSize() == 0) {
        std::cerr << "Polygon_DrawingAlgorithm::draw : polygon has zero points" << std::endl;
        return; // Can't draw a polygon with no vertices
    }
    this->runAlgorithm(inputShape.clone(), sw); // Clone so original is unmodified; pass to algorithm
}
