// ============================================================
// square_point_clipping_algorithm.cpp — Point Clipping Against a Square
//
// CORE IDEA: Identical to rectangle point clipping — a point is
// visible only if it lies inside the square clipping region.
// The square's isInside() checks: left ≤ x ≤ right AND top ≤ y ≤ bottom.
// ============================================================

#include "square_point_clipping_algorithm.h" // Header declaring this class

// Constructor
Square_Point_ClippingAlgorithm::Square_Point_ClippingAlgorithm()
    : ClippingAlgorithm("Square_Point_ClippingAlgorithm") {
}

// ── Core algorithm ────────────────────────────────────────────
void Square_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, Square* square, ScreenWriter *sw) const {
    sw->activate(); // Acquire Windows DC

    if (square->isInside(polygon->points[0])) {
        // The single point is inside the square clipping region → draw it
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    // If outside: silently discard

    sw->deactivate(); // Release Windows DC
}

// ── Public entry point ────────────────────────────────────────
void Square_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputShape.getSize() != 1) {
        std::cerr << "Square_Point_ClippingAlgorithm::clip : shape to draw must be Polygon of 1 point" << std::endl;
        return; // Only works for single-point shapes (a "point" shape)
    }
    if (inputRegion.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_Point_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }

    Shape*  polygon = inputShape.clone();                        // Clone so original is unmodified
    Square* square  = dynamic_cast<Square*>(inputRegion.clone()); // Clone + cast to Square*
    this->runAlgorithm(polygon, square, sw);
}
