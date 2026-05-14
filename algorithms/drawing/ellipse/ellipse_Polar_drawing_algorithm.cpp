// ============================================================
// ellipse_Polar_drawing_algorithm.cpp — Polar Ellipse Algorithm
//
// CORE IDEA: Use the ellipse's parametric (polar) form:
//   x(θ) = xc + a * cos(θ)
//   y(θ) = yc + b * sin(θ)
// where a = horizontal radius, b = vertical radius, θ from 0 to 2π.
//
// This is the direct polar equivalent for an ellipse — the same
// approach as circle_polar but with separate radii for x and y.
//
// The step size is 1 / max(a, b) so that the longer axis never
// has gaps (the shorter axis gets some slight oversampling, which
// is harmless). This matches the circle polar logic: 1/r ≈ 1 pixel.
//
// Cost: calls cos() and sin() every iteration — accurate but slow.
// ============================================================

#include "ellipse_polar_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                      // All standard headers (cos, sin, round, max)

// Constructor
Ellipse_Polar_DrawingAlgorithm::Ellipse_Polar_DrawingAlgorithm()
    : DrawingAlgorithm("Ellipse_Polar_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Ellipse_Polar_DrawingAlgorithm::runAlgorithm(EllipseShape* ellipse, ScreenWriter *sw) const {
    Point p0(ellipse->points[0]); // Center of the ellipse
    Point p1(ellipse->points[1]); // Point defining the horizontal radius
    Point p2(ellipse->points[2]); // Point defining the vertical radius

    double a = p0.euclideanDistance(p1); // Horizontal semi-axis a: distance from center to the x-axis edge point
    double b = p0.euclideanDistance(p2); // Vertical semi-axis b: distance from center to the y-axis edge point

    if(a < 1.0 || b < 1.0) {
        std::cerr << "Ellipse_Polar_DrawingAlgorithm::runAlgorithm : semi-axes must be greater than 0" << std::endl;
        return;
    }

    double step = 1.0 / std::max(a, b); // Angular step = 1 / largest radius
                                         // Ensures no pixel gaps along the longer arc (similar to circle polar)

    sw->activate(); // Acquire Windows DC

    // Sweep θ from 0 to 2π (full revolution)
    for (double radian = 0; radian < 2*3.14159265358979323846; radian += step) {
        double x = (p0.x + a * cos(radian)); // x(θ) = xc + a*cos(θ)  ← parametric formula for x
        double y = (p0.y + b * sin(radian)); // y(θ) = yc + b*sin(θ)  ← parametric formula for y
        sw->setPixel(round(x), round(y), ellipse->borderColor); // round() snaps float to nearest pixel
    }

    sw->deactivate(); // Release Windows DC
}

// ── Public entry point ────────────────────────────────────────
void Ellipse_Polar_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_ELLIPSE) {
        std::cerr << "Ellipse_Polar_DrawingAlgorithm::draw : shape to draw must be Ellipse" << std::endl;
        return;
    }
    EllipseShape* ellipse = dynamic_cast<EllipseShape*>(inputShape.clone()); // Clone + cast
    this->runAlgorithm(ellipse, sw);
}
