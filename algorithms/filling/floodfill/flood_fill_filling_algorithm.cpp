// ============================================================
// flood_fill_filling_algorithm.cpp — Recursive Flood Fill
//
// CORE IDEA: Flood fill works like pouring paint into a region.
// Starting from the pixel the user right-clicked:
//   1. Read the color at that pixel ("fillable color").
//   2. Paint it with the fill color.
//   3. Recursively spread to all 4 neighbors (up, down, left, right)
//      as long as they also have the "fillable color".
//
// The fill stops at shape boundaries because those pixels have
// a different color (the border color), not the fillable color.
//
// DISADVANTAGE: Recursion depth = number of pixels filled.
// For large shapes this can overflow the call stack and crash.
// That's why the iterative version (iterive_flood_fill) exists.
// ============================================================

#include "flood_fill_filling_algorithm.h" // Header declaring this class

// Constructor — registers the display name with the base FillingAlgorithm class
FloodFill_FillingAlgorithm::FloodFill_FillingAlgorithm() : FillingAlgorithm("FloodFill_FillingAlgorithm") {

}

// ── Public entry point called by AppManager ───────────────────
void FloodFill_FillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    // If a clipping region exists, only fill if the start point is inside it
    if (&clippingRegion != nullptr && !clippingRegion.isInside(startPoint)) {
        return; // User clicked outside the allowed clipping region — do nothing
    }

    sw->activate(); // Acquire the Windows drawing context before reading/writing pixels

    COLORREF fillableColor = sw->getPixel(startPoint.x, startPoint.y); // Read the color at the clicked pixel
                                                                         // This is the color we'll replace with the fill color
                                                                         // (usually the background color inside the shape)

    fill_helper(shape, clippingRegion, startPoint, fillableColor, sw); // Begin the recursive flood from the start point

    sw->deactivate(); // Release the drawing context
}

// ── Recursive helper ──────────────────────────────────────────
void FloodFill_FillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, COLORREF fillableColor, ScreenWriter *sw) const {
    COLORREF currentColor = sw->getPixel(startPoint.x, startPoint.y); // Read current pixel color at this point

    // Only fill this pixel if ALL of these conditions are true:
    //   1. shape.isInside(startPoint)            — this pixel is geometrically inside the shape
    //   2. !sw->outOfBounds(startPoint.x, startPoint.y) — it's within the screen boundaries
    //   3. currentColor == fillableColor         — the pixel hasn't been filled yet (still has the original color)
    //   4. currentColor != shape.fillColor       — guard against infinite loop if fillableColor equals fillColor
    //   5. clipping check                        — either no clipping region, or this pixel is inside it
    if( shape.isInside(startPoint)
     && !sw->outOfBounds(startPoint.x, startPoint.y)
     && currentColor == fillableColor
     && currentColor != shape.fillColor
     && (&clippingRegion == nullptr || clippingRegion.isInside(startPoint))) {

        sw->setPixel(startPoint.x, startPoint.y, shape.fillColor); // Paint this pixel with the fill color

        // ── Spread to the 4 cardinal neighbors ───────────────
        // Only recurse into neighbors that are inside the shape and on-screen
        // (the color check happens at the start of the next recursion level)

        Point right(startPoint.x + 1, startPoint.y); // Neighbor one pixel to the right
        if (!sw->outOfBounds(right.x, right.y) && shape.isInside(right)) {
            fill_helper(shape, clippingRegion, right, fillableColor, sw); // Recurse right
        }

        Point left(startPoint.x - 1, startPoint.y);  // Neighbor one pixel to the left
        if (!sw->outOfBounds(left.x, left.y) && shape.isInside(left)) {
            fill_helper(shape, clippingRegion, left, fillableColor, sw);  // Recurse left
        }

        Point down(startPoint.x, startPoint.y + 1);  // Neighbor one pixel below (y increases downward on screen)
        if (!sw->outOfBounds(down.x, down.y) && shape.isInside(down)) {
            fill_helper(shape, clippingRegion, down, fillableColor, sw);  // Recurse down
        }

        Point up(startPoint.x, startPoint.y - 1);    // Neighbor one pixel above
        if (!sw->outOfBounds(up.x, up.y) && shape.isInside(up)) {
            fill_helper(shape, clippingRegion, up, fillableColor, sw);    // Recurse up
        }
    }
    // If none of the conditions above are met, this branch of the recursion simply returns (base case)
}
