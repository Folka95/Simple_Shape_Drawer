// ============================================================
// circle_quarter_with_circle_filling_algorithm.cpp
//
// CORE IDEA: Fill ONE QUARTER of a circle by drawing concentric
// quarter-circles from radius 0 up to the full radius.
//
// The user right-clicks in a specific quadrant of the circle.
// The algorithm:
//   1. Determines WHICH quadrant the click landed in (1–4)
//   2. Sweeps from r=0 to r=radius in steps of 0.3
//   3. For each r, draws a quarter arc using the polar formula
//      in the appropriate angular range (0-π/2, π/2-π, etc.)
//
// QUADRANT MAPPING (matching screen coordinates where y grows down):
//   Q1 → angles 0     to π/2    (right-bottom quadrant on screen)
//   Q2 → angles π/2   to π      (left-bottom)
//   Q3 → angles π     to 3π/2   (left-top)
//   Q4 → angles 3π/2  to 2π     (right-top)
//
// By drawing only the selected quarter, this acts as a selective
// quarter-fill (useful for pie charts or decorative arcs).
// ============================================================

#include "circle_quarter_with_circle_filling_algorithm.h" // Header declaring this class

// Constructor
Circle_QuarterWithCircle_FillingAlgorithm::Circle_QuarterWithCircle_FillingAlgorithm()
    : FillingAlgorithm("Circle_QuarterWithCircle_FillingAlgorithm") {

}

// ── Draw one quarter-arc at a given radius ────────────────────
// xc, yc = center of the circle
// radius  = radius of this concentric arc
// q       = which quarter (1–4)
// color   = fill color
void Circle_QuarterWithCircle_FillingAlgorithm::drawQuarterCircle(double xc, double yc, double radius, int q, ScreenWriter *sw, COLORREF color, const Shape &clippingRegion) const {
    double step = 1.0 / radius; // Angular step ≈ 1 pixel of arc (same as polar circle algorithm)

    double start = 0;
    double end   = M_PI / 2; // Default: first quadrant range

    // Set the angular range based on which quadrant was selected
    if      (q == 1) { start = 0;           end = M_PI / 2;       } // Quadrant 1: 0° to 90°
    else if (q == 2) { start = M_PI / 2;    end = M_PI;            } // Quadrant 2: 90° to 180°
    else if (q == 3) { start = M_PI;        end = 3 * M_PI / 2;    } // Quadrant 3: 180° to 270°
    else if (q == 4) { start = 3 * M_PI / 2; end = 2 * M_PI;      } // Quadrant 4: 270° to 360°

    for (double radian = start; radian < end; radian += step) {
        double x = (xc + radius * cos(radian)); // x = xc + r*cos(θ) — polar-to-Cartesian
        double y = (yc + radius * sin(radian)); // y = yc + r*sin(θ)
        if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
            sw->setPixel(round(x), round(y), color); // Draw if inside clipping region (or no clipping)
        }
    }
}

// ── Fill the selected quarter by stacking concentric arcs ─────
void Circle_QuarterWithCircle_FillingAlgorithm::fill_helper(const Circle &circle, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    int q = circle.scanQuarter(startPoint); // Determine which quarter the user right-clicked inside

    if (q == 0 || q == 5) {
        std::cerr << "Circle_QuarterWithCircle_FillingAlgorithm::fill_helper : Please Choose a valid quarter" << std::endl;
        return; // 0 = on the boundary exactly, 5 = outside the circle — invalid targets
    }

    Point center = circle.getCenter();  // Center point of the circle
    double radius = circle.getRadius(); // Full radius of the circle

    // Draw concentric quarter-arcs from r=0 to r=radius in steps of 0.3
    // Smaller step = denser fill, fewer gaps
    for (double r = 0; r < radius; r += 0.3) {
        drawQuarterCircle(center.x, center.y, r, q, sw, circle.fillColor, clippingRegion);
    }
}

// ── Public entry point ────────────────────────────────────────
void Circle_QuarterWithCircle_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_CIRCLE) {
        std::cerr << "Circle_QuarterWithCircle_FillingAlgorithm::fill : Region must be a Circle" << std::endl;
        return;
    }
    const Circle* circle = dynamic_cast<const Circle*>(&shape); // Cast to Circle* to access circle-specific methods
    if (!circle) return;

    sw->activate(); // Acquire Windows DC
    fill_helper(*circle, clippingRegion, startPoint, sw); // Run the quarter fill
    sw->deactivate(); // Release Windows DC
}
