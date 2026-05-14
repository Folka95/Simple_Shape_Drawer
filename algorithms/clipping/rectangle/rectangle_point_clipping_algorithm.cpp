// ============================================================
// rectangle_point_clipping_algorithm.cpp — Point Clipping Against a Rectangle
//
// CORE IDEA: The simplest possible clipping algorithm.
// A point is "visible" only if it falls INSIDE the clipping
// rectangle. If it does, draw it. If not, discard it.
//
// The isInside() check on the rectangle tests:
//   left ≤ x ≤ right  AND  top ≤ y ≤ bottom
//
// This is used when the selected shape type is a single point (polygon of 1 point).
// ============================================================

#include "rectangle_point_clipping_algorithm.h" // Header declaring this class

// Constructor — registers the display name with the base ClippingAlgorithm class
Rectangle_Point_ClippingAlgorithm::Rectangle_Point_ClippingAlgorithm()
    : ClippingAlgorithm("Rectangle_Point_ClippingAlgorithm") {}

// ── Core algorithm ────────────────────────────────────────────
void Rectangle_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, RectangleShape* rectangle, ScreenWriter *sw) const {
    sw->activate(); // Acquire the Windows drawing context

    // The only test: is this single point inside the rectangle?
    if (rectangle->isInside(polygon->points[0])) {
        // Point is inside the clipping region → draw it
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    // If NOT inside: silently discard (do not draw anything)

    sw->deactivate(); // Release the drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Rectangle_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    // Guard: only handles a polygon with exactly 1 point (a "point" shape)
    if (inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputShape.getSize() != 1) {
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon of 1 point" << std::endl;
        return; // This algorithm only clips single points, not multi-vertex polygons
    }
    if (inputRegion.getType() != SHAPE_RECTANGLE) {
        std::cerr << "Rectangle_Point_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return; // The clipping boundary must be a rectangle for this algorithm
    }

    Shape* polygon = inputShape.clone(); // Clone so we don't modify the original
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputRegion.clone()); // Clone + cast to typed pointer

    this->runAlgorithm(polygon, rectangle, sw); // Run the single-point test
}
