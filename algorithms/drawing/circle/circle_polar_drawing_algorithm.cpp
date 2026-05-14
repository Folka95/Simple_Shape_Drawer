// ============================================================
// circle_polar_drawing_algorithm.cpp — Polar Circle Algorithm
//
// CORE IDEA: Uses trigonometry (polar coordinates) directly.
// A circle with center (xc, yc) and radius r can be written as:
//   x(θ) = xc + r * cos(θ)
//   y(θ) = yc + r * sin(θ)
// where θ (theta) goes from 0 to 2π (a full revolution).
//
// We step θ in increments of (1/r) radians.
// Why 1/r? The arc length per radian is r, so stepping 1/r
// radians at a time moves approximately 1 pixel along the
// circle — ensuring no gaps and no double-drawing.
//
// This is the most intuitive algorithm but slowest because
// it calls sin() and cos() every step (expensive trig).
// The Midpoint algorithm avoids trig entirely.
// ============================================================

#include "circle_polar_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                     // All standard headers (sqrt, pow, cos, sin, round)

// Constructor — registers the display name with the base class
Circle_Polar_DrawingAlgorithm::Circle_Polar_DrawingAlgorithm() : DrawingAlgorithm("Circle_Polar_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Circle_Polar_DrawingAlgorithm::runAlgorithm(Circle* circle, ScreenWriter *sw) const {
    Point p0(circle->points[0]); // Center point (user's first click)
    Point p1(circle->points[1]); // Edge point  (user's second click — defines radius)

    // Compute radius = straight-line distance from center to edge
    double radius = sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));

    if(radius < 1.0) {
        // Can't draw a circle with zero or sub-pixel radius — abort cleanly
        std::cerr << "Circle_Polar_DrawingAlgorithm::runAlgorithm : radius must be greater than 0" << std::endl;
        return;
    }

    double step = 1.0 / radius; // Angular increment per iteration (radians).
                                 // Arc length per radian = r, so 1/r radians ≈ 1 pixel of arc.
                                 // Smaller radius → larger angular steps (fewer iterations needed).
                                 // Larger radius  → smaller angular steps (more iterations for full circle).

    sw->activate(); // Acquire the Windows drawing context before writing pixels

    // Loop θ from 0 all the way around to 2π (one full circle)
    for (double radian = 0; radian < 2*3.14159265358979323846; radian += step) {
        // 3.14159... is π hardcoded to avoid needing M_PI (which is non-standard)
        double x = (p0.x + radius * cos(radian)); // x = center_x + r * cos(θ)  ← polar-to-Cartesian conversion
        double y = (p0.y + radius * sin(radian)); // y = center_y + r * sin(θ)  ← polar-to-Cartesian conversion
        sw->setPixel(round(x), round(y), circle->borderColor); // round() maps the float position to the nearest integer pixel
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Circle_Polar_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_Polar_DrawingAlgorithm::draw : shape to draw must be Circle" << std::endl;
        return;
    }
    Circle* circle = dynamic_cast<Circle*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(circle, sw);
}
