// ============================================================
// rectangle_drawing_algorithm.cpp — Rectangle Drawing Algorithm
//
// CORE IDEA: A rectangle is drawn by connecting its 4 corners
// with straight line segments. The 4 corners are:
//   TopLeft → TopRight    (top edge)
//   TopRight → BottomRight (right edge)
//   BottomRight → BottomLeft (bottom edge)
//   TopLeft → BottomLeft  (left edge)
//
// Each edge is drawn using the Midpoint line algorithm.
// The RectangleShape provides helper methods (getTopLeft, etc.)
// that compute the corners from the two stored defining points.
// ============================================================

#include "rectangle_drawing_algorithm.h"                     // Header declaring this class
#include "../line/line_midpoint_drawing_algorithm.h"          // Bresenham midpoint line algorithm for each edge
#include "../../../core/shapes/line.h"                        // Line shape — temporary container for each edge

// Constructor
Rectangle_DrawingAlgorithm::Rectangle_DrawingAlgorithm()
    : DrawingAlgorithm("Rectangle_DrawingAlgorithm") {}

// ── Helper: draw one edge of the rectangle ────────────────────
void Rectangle_DrawingAlgorithm::drawLine(const Point& p1, const Point& p2, ScreenWriter *sw) const {
    Line* line = new Line();  // Create a temporary Line shape for this edge
    line->addPoint(p1);       // Start corner of the edge
    line->addPoint(p2);       // End corner of the edge
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm(); // Use Bresenham
    line_midpoint_drawing_algorithm->draw(*line, sw); // Draw the edge to the screen
}

// ── Core algorithm: draw all 4 edges ──────────────────────────
void Rectangle_DrawingAlgorithm::runAlgorithm(RectangleShape* rectangle, ScreenWriter *sw) const {
    // The rectangle provides computed corner accessors based on its two stored defining points
    drawLine(rectangle->getTopLeft(),     rectangle->getTopRight(),    sw); // Top edge:    left→right
    drawLine(rectangle->getTopRight(),    rectangle->getBottomRight(), sw); // Right edge:  top→bottom
    drawLine(rectangle->getBottomRight(), rectangle->getBottomLeft(),  sw); // Bottom edge: right→left
    drawLine(rectangle->getTopLeft(),     rectangle->getBottomLeft(),  sw); // Left edge:   top→bottom
}

// ── Public entry point called by AppManager ───────────────────
void Rectangle_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_RECTANGLE) {
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape to draw must be Rectangle" << std::endl;
        return;
    }
    if(!inputShape.isEnoughToDraw()) {
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape is not enough to draw" << std::endl;
        return; // Need both defining corners before we can draw — abort if only one was clicked
    }
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputShape.clone()); // Clone + cast
    this->runAlgorithm(rectangle, sw);
}
