// ============================================================
// iterive_flood_fill_filling_algorithm.cpp — Iterative Flood Fill
//
// CORE IDEA: Same flood fill logic as the recursive version,
// but uses an explicit stack (std::stack) instead of the CPU
// call stack. This prevents stack overflow crashes when filling
// large shapes, because the heap (where std::stack lives) is
// much larger than the CPU call stack.
//
// HOW IT WORKS:
//   1. Push the start point onto the stack.
//   2. Loop: pop a point, check if it should be filled.
//   3. If yes: paint it, then push all 4 neighbors onto the stack.
//   4. Repeat until the stack is empty (no more points to fill).
//
// The behavior is identical to recursive flood fill, just the
// order of processing may differ (stack → depth-first vs queue → BFS).
// ============================================================

#include "iterive_flood_fill_filling_algorithm.h" // Header declaring this class

// Constructor — registers the display name
IterFloodFill_FillingAlgorithm::IterFloodFill_FillingAlgorithm() : FillingAlgorithm("IterFloodFill_FillingAlgorithm") {

}

// ── Main fill function ────────────────────────────────────────
void IterFloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    // If a clipping region is active, only fill if the start point lies inside it
    if (&clippingRegion != nullptr && !clippingRegion.isInside(startPoint)) {
        return; // Outside the allowed region — abort
    }

    sw->activate(); // Acquire the Windows drawing context

    std::stack<Point> stack; // An explicit stack that holds Points waiting to be processed
                              // Replaces the recursive function call stack

    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y); // The original color to be replaced
                                                                         // (the interior background color before filling)

    stack.push(startPoint); // Seed the fill: put the user's clicked point onto the stack first

    // ── Main loop ─────────────────────────────────────────────
    while(!stack.empty()) { // Keep processing until no more points remain
        Point current = stack.top(); // Read the next point from the top of the stack (most recently added)
        stack.pop();                 // Remove it from the stack now that we've claimed it

        if(sw->outOfBounds(current.x, current.y)) {
            continue; // Skip this point — it's off-screen; jumping to the next iteration avoids an invalid pixel read
        }

        COLORREF currentColor = sw->getPixel(current.x, current.y); // Read this pixel's current color

        // Only paint this pixel if ALL conditions hold:
        //   1. shape.isInside(current)          — geometrically inside the shape
        //   2. currentColor == fillableColor     — not yet painted (still the original background color)
        //   3. currentColor != shape.borderColor — not on the shape's drawn outline
        //   4. currentColor != shape.fillColor   — not already painted (avoids infinite re-queueing)
        //   5. clipping region check             — either no clipping, or inside the allowed region
        if(shape.isInside(current)
        && currentColor == fillableColor
        && currentColor != shape.borderColor
        && currentColor != shape.fillColor
        && (&clippingRegion == nullptr || clippingRegion.isInside(current))) {

            sw->setPixel(current.x, current.y, shape.fillColor); // Paint this pixel with the fill color

            // ── Queue the 4 cardinal neighbors for processing ──
            // We push them only if they are on-screen and inside the shape,
            // to avoid wasting stack space on obviously invalid points

            Point right(current.x + 1, current.y); // One pixel to the right
            if (!sw->outOfBounds(right.x, right.y) && shape.isInside(right)) {
                stack.push(right); // Add right neighbor to be processed later
            }

            Point left(current.x - 1, current.y);  // One pixel to the left
            if (!sw->outOfBounds(left.x, left.y) && shape.isInside(left)) {
                stack.push(left);
            }

            Point up(current.x, current.y + 1);    // One pixel below (y grows downward on screen)
            if (!sw->outOfBounds(up.x, up.y) && shape.isInside(up)) {
                stack.push(up);
            }

            Point down(current.x, current.y - 1);  // One pixel above
            if (!sw->outOfBounds(down.x, down.y) && shape.isInside(down)) {
                stack.push(down);
            }
        }
        // If conditions not met: pixel is already filled, on the border, or out-of-shape — skip it
    }

    sw->deactivate(); // Release the Windows drawing context
}
