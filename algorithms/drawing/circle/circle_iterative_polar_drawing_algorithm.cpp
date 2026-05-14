// ============================================================
// circle_iterative_polar_drawing_algorithm.cpp — Iterative Polar Circle
//
// CORE IDEA: An improvement over the naive polar algorithm.
// Instead of calling cos() and sin() fresh each iteration,
// this uses a ROTATION MATRIX to incrementally rotate (x, y)
// by deltaTheta each step — avoiding the expensive trig calls
// inside the loop.
//
// Rotation matrix by angle θ:
//   newX =  x * cos(θ) - y * sin(θ)
//   newY =  x * sin(θ) + y * cos(θ)
//
// cos(θ) and sin(θ) are computed ONCE before the loop.
// Each step is just 4 multiplications and 2 additions.
//
// We only sweep the first octant (0 to π/4) and use 8-fold
// symmetry (same as direct/midpoint) to plot the full circle.
// This gives us fast trig-free stepping AND symmetry savings.
// ============================================================

#include "circle_iterative_polar_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                               // All standard headers (sqrt, cos, sin, round)

// Constructor — registers the display name
Circle_iterativePolar_DrawingAlgorithm::Circle_iterativePolar_DrawingAlgorithm()
    : DrawingAlgorithm("Circle_iterativePolar_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Circle_iterativePolar_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]); // Center of the circle
    Point p1(circle->points[1]); // Edge point — defines the radius

    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2)); // Euclidean distance = radius

    if(radius < 1.0) {
        std::cerr << "Circle_iterativePolar_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }

    double deltaTheta = 1.0 / radius; // Angular step size (radians); same as the polar algorithm: 1/r rads ≈ 1 pixel

    double cosDelta = cos(deltaTheta); // Pre-compute cos(Δθ) ONCE — used in every rotation step
    double sinDelta = sin(deltaTheta); // Pre-compute sin(Δθ) ONCE — used in every rotation step

    double x = radius; // Start at the rightmost point of the circle: (r, 0) in local coordinates
    double y = 0;      // y starts at 0 (on the horizontal axis)

    sw->activate(); // Acquire the Windows drawing context

    // Sweep from angle 0 to π/4 (the first octant = 1/8 of the circle).
    // The 8-fold symmetry mirrors each point to cover the full circle.
    for (double radian = 0; radian < 3.14159265358979323846/4; radian += deltaTheta) {

        // Compute the 8 symmetric points from the current (x, y) offset
        // (These are the same 8 mirror positions as the midpoint and direct algorithms)
        Point octet1 = { (p0.x + x), (p0.y + y) }; // ( x, y) — first octant
        Point octet2 = { (p0.x + x), (p0.y - y) }; // ( x,-y)
        Point octet3 = { (p0.x - x), (p0.y + y) }; // (-x, y)
        Point octet4 = { (p0.x - x), (p0.y - y) }; // (-x,-y)
        Point octet5 = { (p0.x + y), (p0.y + x) }; // ( y, x) — diagonal reflection
        Point octet6 = { (p0.x + y), (p0.y - x) }; // ( y,-x)
        Point octet7 = { (p0.x - y), (p0.y + x) }; // (-y, x)
        Point octet8 = { (p0.x - y), (p0.y - x) }; // (-y,-x)

        // Plot all 8 points (round to nearest integer pixel)
        sw->setPixel(round(octet1.x), round(octet1.y), circle->borderColor);
        sw->setPixel(round(octet2.x), round(octet2.y), circle->borderColor);
        sw->setPixel(round(octet3.x), round(octet3.y), circle->borderColor);
        sw->setPixel(round(octet4.x), round(octet4.y), circle->borderColor);
        sw->setPixel(round(octet5.x), round(octet5.y), circle->borderColor);
        sw->setPixel(round(octet6.x), round(octet6.y), circle->borderColor);
        sw->setPixel(round(octet7.x), round(octet7.y), circle->borderColor);
        sw->setPixel(round(octet8.x), round(octet8.y), circle->borderColor);

        // ── Rotate (x, y) by deltaTheta using the rotation matrix ──
        // This avoids recomputing cos(radian) and sin(radian) from scratch each time
        double newX = x * cosDelta - y * sinDelta; // x' = x*cos(Δθ) - y*sin(Δθ)
        double newY = x * sinDelta + y * cosDelta; // y' = x*sin(Δθ) + y*cos(Δθ)
        x = newX; // Advance x to the rotated position
        y = newY; // Advance y to the rotated position
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point ────────────────────────────────────────
void Circle_iterativePolar_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_iterativePolar_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(circle, sw);
}
