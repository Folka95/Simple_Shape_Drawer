// ============================================================
// line_parametric_drawing_algorithm.cpp — Parametric Line Algorithm
//
// CORE IDEA: Express the line as a parametric equation:
//   x(t) = x0 + dx * t
//   y(t) = y0 + dy * t
// where t goes from 0.0 to 1.0.
//
// At t=0 you get the start point exactly.
// At t=1 you get the end point exactly.
// At t=0.5 you get the exact midpoint.
//
// We divide the range [0,1] into (steps) equal increments,
// where steps = max(|dx|, |dy|). This ensures we generate
// at least one pixel per column (or row), so the line has
// no gaps regardless of angle.
//
// This is the simplest algorithm to understand but uses
// floating-point multiply every step (slower than Bresenham).
// ============================================================

#include "line_parametric_drawing_algorithm.h" // Header declaring this class
#include <bits/stdc++.h>                        // All standard headers (max, abs, round)

// Constructor — registers the display name with the base DrawingAlgorithm class
Line_Parametric_DrawingAlgorithm::Line_Parametric_DrawingAlgorithm() : DrawingAlgorithm("Line_Parametric_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Line_Parametric_DrawingAlgorithm::runAlgorithm(Line* line, ScreenWriter *sw) const {
    Point p0(line->points[0]); // Start point (t = 0)
    Point p1(line->points[1]); // End point   (t = 1)

    int dx = p1.x - p0.x; // Total x displacement from start to end (can be negative)
    int dy = p1.y - p0.y; // Total y displacement from start to end (can be negative)

    // Choose the number of steps = the longer axis span.
    // Using max(|dx|, |dy|) guarantees no pixel gaps:
    //  - If the line is mostly horizontal, |dx| is larger → we step by 1/|dx| each time, never skipping a column
    //  - If the line is mostly vertical,   |dy| is larger → we step by 1/|dy| each time, never skipping a row
    int steps = std::max(abs(dx), abs(dy));

    sw->activate(); // Acquire the Windows drawing context before writing pixels

    for(int i = 0; i <= steps; i++) {       // Iterate from step 0 (start) to step `steps` (end), inclusive
        double t = (double)i / steps;        // t goes from 0.0 to 1.0 uniformly; each step is 1/steps apart
        double x = p0.x + dx * t;           // Parametric x: start x + (total x span) * how far along we are
        double y = p0.y + dy * t;           // Parametric y: start y + (total y span) * how far along we are
        sw->setPixel(round(x), round(y), line->borderColor); // round() maps the float position to the nearest pixel
    }

    sw->deactivate(); // Release the Windows drawing context
}

// ── Public entry point called by AppManager ───────────────────
void Line_Parametric_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    // Guard: this algorithm only handles Line shapes
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Line_Parametric_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone()); // Clone so original is unmodified; cast to typed pointer
    this->runAlgorithm(line, sw); // Run the parametric algorithm
}
