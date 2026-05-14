// ============================================================
// convex_filling_algorithm.cpp — Scanline Fill for CONVEX Polygons
//
// CORE IDEA: Instead of flood-filling pixel-by-pixel from a
// seed point, scanline filling processes the shape one
// HORIZONTAL ROW (scanline) at a time.
//
// For each row y:
//   1. Find the leftmost x (xmin) and rightmost x (xmax)
//      where the polygon boundary crosses that row.
//   2. Draw a horizontal line from xmin to xmax.
//
// This is done by scanning every EDGE of the polygon and
// recording for each y-row which x values the edge passes through.
//
// WHY "CONVEX ONLY" here: this simpler version tracks only one
// xmin and one xmax per row. Convex shapes guarantee exactly
// two boundary crossings per row, so xmin/xmax is sufficient.
// For shapes with re-entrant (concave) sections, the Non-Convex
// version handles the more complex multiple-crossing case.
//
// TIME COMPLEXITY: O(E * H) where E = edges, H = height of shape.
// Much faster than flood fill for large shapes.
// ============================================================

#include "convex_filling_algorithm.h" // Header declaring this class

// ── File-local helpers (anonymous namespace = private to this file) ──
namespace {

    // Entry: stores the leftmost and rightmost x for one row of the scanline table
    struct Entry {
        int xmin, xmax;
    };

    // Initialize every entry to "no pixels found yet":
    //   xmin = very large positive (so any real x will replace it)
    //   xmax = very large negative (so any real x will replace it)
    void InitEntries(std::vector<Entry>& table) {
        for(size_t i = 0; i < table.size(); i++) {
            table[i].xmin =  INT_MAX;  // Sentinel: no left boundary found yet for this row
            table[i].xmax = -INT_MAX;  // Sentinel: no right boundary found yet for this row
        }
    }

    // Scan a single polygon EDGE from vertex v1 to v2 and update the table.
    // For each integer y-row that this edge passes through, record
    // the x value of the edge at that row into xmin/xmax.
    void ScanEdge(Point v1, Point v2, std::vector<Entry>& table, int minY) {
        if((int)v1.y == (int)v2.y) return; // Horizontal edge — contributes no scanline crossings, skip it

        if(v1.y > v2.y) std::swap(v1, v2); // Ensure v1 is always the lower y (top of screen); makes iteration direction consistent

        // Slope of the edge in terms of x per unit y:
        //   minv = (x2 - x1) / (y2 - y1)
        //   This is the "inverse slope" (Δx per Δy).
        //   As y increases by 1 (one row down), x increases by minv.
        double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);

        double x = v1.x; // Starting x at the top of this edge
        int y    = (int)v1.y; // Starting y (integer row)
        int endY = (int)v2.y; // Ending y — we stop before reaching endY (the bottom vertex is shared with the next edge)

        while(y < endY) { // Step down one row at a time through this edge
            int idx = y - minY; // Convert absolute screen y to index into our table (table[0] = row minY)
            if (idx >= 0 && idx < (int)table.size()) {
                if(x < table[idx].xmin) table[idx].xmin = (int)std::ceil(x);  // Record leftmost x (ceil: stay inside the shape)
                if(x > table[idx].xmax) table[idx].xmax = (int)std::floor(x); // Record rightmost x (floor: stay inside the shape)
            }
            y++;      // Move to the next row
            x += minv; // Advance x along the edge slope
        }
    }
}

// Constructor
ConvexFillingAlgorithm::ConvexFillingAlgorithm() : FillingAlgorithm("ConvexFillingAlgorithm") {

}

// ── Public entry point ────────────────────────────────────────
void ConvexFillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    // Respect the clipping region: don't fill if start point is outside it
    if (&clippingRegion != nullptr && !clippingRegion.isInside(startPoint)) {
        return;
    }
    sw->activate(); // Acquire the drawing context
    fill_helper(shape, clippingRegion, startPoint, sw); // Do the actual scanline fill
    sw->deactivate(); // Release the drawing context
}

// ── Scanline fill implementation ──────────────────────────────
void ConvexFillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    std::vector<Point> pts = shape.getSidePoints(); // Get the polygon's boundary vertices
    if (pts.empty() && !shape.points.empty()) {
        pts = shape.points; // Fallback: if no pre-computed side points, use the raw defining points
    }

    if (pts.size() < 3) return; // Need at least 3 vertices to form a polygon worth filling

    // ── STEP 1: Find the vertical range of the shape ──────────
    int minY =  INT_MAX; // Will hold the topmost (smallest) y value among all vertices
    int maxY = -INT_MAX; // Will hold the bottommost (largest) y value among all vertices
    for (const auto& p : pts) {
        if ((int)p.y < minY) minY = (int)p.y; // Update top boundary
        if ((int)p.y > maxY) maxY = (int)p.y; // Update bottom boundary
    }

    if (minY > maxY) return; // Degenerate case: all points at the same y — nothing to fill

    int numEntries = maxY - minY + 2; // Number of rows in our scanline table (+2 for a small safety margin)
    if (numEntries <= 0 || numEntries > 10000) return; // Sanity check: reject impossibly large or degenerate shapes

    // ── STEP 2: Initialize the scanline table ─────────────────
    std::vector<Entry> table(numEntries); // One Entry per y-row between minY and maxY
    InitEntries(table); // Set all xmin/xmax to sentinel values

    // ── STEP 3: Scan every edge of the polygon ────────────────
    // We walk the polygon edges in order: last→0, 0→1, 1→2, ..., n-2→n-1
    Point v1 = pts.back(); // Start with the last vertex as the "previous" vertex
    for(size_t i = 0; i < pts.size(); i++) {
        Point v2 = pts[i];           // Current vertex
        ScanEdge(v1, v2, table, minY); // Record where this edge crosses each row
        v1 = pts[i];                 // Advance: current becomes "previous" for next iteration
    }

    // ── STEP 4: Draw horizontal scanlines between xmin and xmax ──
    COLORREF color = shape.fillColor; // The fill color chosen by the user

    for(int i = 0; i < numEntries; i++) {
        if(table[i].xmin <= table[i].xmax) { // Only draw if valid bounds were recorded for this row
            int y = i + minY; // Convert table index back to actual screen y coordinate

            for(int x = table[i].xmin; x <= table[i].xmax; x++) { // Fill every pixel in this horizontal span
                // Respect the clipping region: only draw if within it (or if no clipping region)
                if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
                    sw->setPixel(x, y, color); // Paint this pixel
                }
            }
        }
    }
}
