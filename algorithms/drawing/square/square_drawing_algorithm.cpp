// ============================================================
// square_drawing_algorithm.cpp — Square Drawing Algorithm
//
// CORE IDEA: A square is drawn by connecting its 4 corners
// with 4 straight line segments (same as rectangle drawing).
// The Square shape enforces equal width and height, so the
// second defining point is adjusted to create a perfect square.
//
// Corners are accessed via getTopLeft/TopRight/BottomLeft/BottomRight
// helper methods on the Square shape.
// ============================================================

#include "square_drawing_algorithm.h"                       // Header declaring this class
#include "../line/line_midpoint_drawing_algorithm.h"         // Bresenham line drawer for each edge
#include "../../../core/shapes/line.h"                       // Line shape — temporary container per edge

// Constructor
Square_DrawingAlgorithm::Square_DrawingAlgorithm()
    : DrawingAlgorithm("Square_DrawingAlgorithm") {}

// ── Helper: draw one edge of the square ──────────────────────
void Square_DrawingAlgorithm::drawLine(const Point& p1, const Point& p2, ScreenWriter *sw) const {
    Line* line = new Line();  // Temporary line shape
    line->addPoint(p1);       // Start corner
    line->addPoint(p2);       // End corner
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw); // Draw using Bresenham
}

// ── Core algorithm: draw all 4 sides ──────────────────────────
void Square_DrawingAlgorithm::runAlgorithm(Square* square, ScreenWriter *sw) const {
    drawLine(square->getTopLeft(),     square->getTopRight(),    sw); // Top edge
    drawLine(square->getTopRight(),    square->getBottomRight(), sw); // Right edge
    drawLine(square->getBottomRight(), square->getBottomLeft(),  sw); // Bottom edge
    drawLine(square->getTopLeft(),     square->getBottomLeft(),  sw); // Left edge
}

// ── Public entry point ────────────────────────────────────────
void Square_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_DrawingAlgorithm::draw : shape to draw must be Square" << std::endl;
        return;
    }
    if(!inputShape.isEnoughToDraw()) {
        std::cerr << "Square_DrawingAlgorithm::draw : shape is not enough to draw" << std::endl;
        return; // Square needs both defining points before it can be drawn
    }
    Square* square = dynamic_cast<Square*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(square, sw);
}
