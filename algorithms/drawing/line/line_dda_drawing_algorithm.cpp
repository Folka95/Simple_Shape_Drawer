// ============================================================
// line_dda_drawing_algorithm.cpp — DDA Line Drawing Algorithm
//
// DDA = Digital Differential Analyzer.
// CORE IDEA: A line from (x0,y0) to (x1,y1) has a constant
// slope m = dy/dx. Instead of computing y = mx + b for every
// x (which involves expensive floating-point multiply each
// step), DDA starts at one endpoint and repeatedly ADDS the
// slope increment to y as it steps one pixel at a time.
// This turns per-pixel multiply into per-pixel add — faster.
//
// Limitation vs Midpoint: uses floating-point arithmetic and
// round(), which is slightly slower but simpler to understand.
// ============================================================

#include "line_dda_drawing_algorithm.h" // Header that declares this class
#include <bits/stdc++.h>                // All standard headers (abs, round, etc.)

// Constructor — passes the algorithm name up to the base class so it can be displayed in the status printout
Line_DDA_DrawingAlgorithm::Line_DDA_DrawingAlgorithm() : DrawingAlgorithm("Line_DDA_DrawingAlgorithm") {

}

// ── Core algorithm ────────────────────────────────────────────
void Line_DDA_DrawingAlgorithm::runAlgorithm(Line* line, ScreenWriter *sw) const {
    Point p0(line->points[0]); // Copy of the first endpoint (start of the line)
    Point p1(line->points[1]); // Copy of the second endpoint (end of the line)

    int dx = p0.dx(line->points[1]); // Total horizontal span: x1 - x0 (positive = going right)
    int dy = p0.dy(line->points[1]); // Total vertical span:   y1 - y0 (positive = going down)

    sw->activate(); // Acquire a Windows device context (DC) so we can draw pixels to the screen

    // ── Edge case: both endpoints are the same pixel ──────────
    if(dx == 0 && dy == 0) {
        sw->setPixel(p0.x, p0.y, line->borderColor); // Just draw the single point and stop
        return;
    }

    sw->setPixel(p0.x, p0.y, line->borderColor); // Draw the very first pixel (starting endpoint)

    // ── Case 1: line is more horizontal than vertical (|dx| >= |dy|) ──
    // Step one pixel at a time along X (the longer axis), compute Y by adding the slope fraction
    if(abs(dx) >= abs(dy)) {
        int x = p0.x;                      // Current integer x — will walk one pixel at a time toward x1
        int xinc = dx > 0 ? 1 : -1;        // Step direction: +1 if line goes right, -1 if it goes left
        double y = p0.y;                   // Current y as a float so fractional accumulation doesn't get lost
        double yinc = (double)dy / dx * xinc; // How much y changes per single x-step = slope × x-direction
                                              // Multiply by xinc so the sign is correct regardless of direction

        while(x != p1.x) {         // Keep stepping until we reach the last x pixel
            x += xinc;             // Move one pixel in the x direction
            y += yinc;             // Accumulate the fractional y change
            sw->setPixel(x, round(y), line->borderColor); // round(y) converts float to nearest integer pixel row
        }
    }
    // ── Case 2: line is more vertical than horizontal (|dy| > |dx|) ──
    // Mirror of Case 1: step along Y instead to avoid gaps in steep lines
    else {
        int y = p0.y;                      // Current integer y — steps one pixel at a time toward y1
        int yinc = dy > 0 ? 1 : -1;        // Step direction: +1 down, -1 up
        double x = p0.x;                   // Current x as a float for fractional accumulation
        double xinc = (double)dx / dy * yinc; // How much x changes per single y-step = 1/slope × y-direction

        while(y != p1.y) {         // Keep stepping until we reach the last y pixel
            x += xinc;             // Accumulate fractional x change
            y += yinc;             // Move one pixel in the y direction
            sw->setPixel(round(x), y, line->borderColor); // round(x) maps float to nearest integer pixel column
        }
    }

    sw->deactivate(); // Release the Windows device context — must always pair with activate()
}

// ── Public entry point called by AppManager ───────────────────
void Line_DDA_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    // Safety check: this algorithm only works on Line shapes
    if(inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Line_DDA_DrawingAlgorithm::draw : shape to draw must be Line" << std::endl;
        return; // Bail out early rather than crash on a wrong-type cast below
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone()); // Clone the shape so we don't mutate the original
                                                           // dynamic_cast converts the base Shape* to a Line*
    this->runAlgorithm(line, sw); // Hand the typed pointer off to the actual algorithm
}
