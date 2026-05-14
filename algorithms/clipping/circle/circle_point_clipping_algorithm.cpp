// ============================================================
// circle_point_clipping_algorithm.cpp — Point Clipping Against a Circle
//
// CORE IDEA: A point is visible only if it lies INSIDE the
// circular clipping region. The test is:
//   (x - cx)² + (y - cy)² ≤ r²
// which is exactly what circle->isInside() performs.
//
// If the point passes, draw it. If not, discard it silently.
// This is the circular analog of rectangle_point_clipping.
// ============================================================

#include "circle_point_clipping_algorithm.h" // Header declaring this class

// Constructor — registers the display name
Circle_Point_ClippingAlgorithm::Circle_Point_ClippingAlgorithm()
    : ClippingAlgorithm("Circle_Point_ClippingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Circle_Point_ClippingAlgorithm::runAlgorithm(Shape* polygon, Circle* circle, ScreenWriter *sw) const {
    sw->activate(); // Acquire Windows DC

    if(circle->isInside(polygon->points[0])) {
        // The single point lies inside the circle → draw it
        // Uses isInside() which tests: distance from center ≤ radius
        sw->setPixel(polygon->points[0].x, polygon->points[0].y, polygon->borderColor);
    }
    // If outside the circle: silently discard (don't draw)

    sw->deactivate(); // Release Windows DC
}

// ── Public entry point ────────────────────────────────────────
void Circle_Point_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Circle_Point_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }
    if(inputRegion.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Point_ClippingAlgorithm::clip : region must be Circle" << std::endl;
        return; // Clipping region must be a circle for this algorithm
    }
    Shape*  polygon = inputShape.clone();                           // Clone + keep as base Shape*
    Circle* circle  = dynamic_cast<Circle*>(inputRegion.clone());  // Clone + cast to Circle* for isInside()
    this->runAlgorithm(polygon, circle, sw);
}
