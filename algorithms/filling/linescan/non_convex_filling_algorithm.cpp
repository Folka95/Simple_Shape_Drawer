// ============================================================
// non_convex_filling_algorithm.cpp — Scanline Fill for NON-CONVEX Polygons
//                                    (uses an Edge Table + Active Edge List)
//
// CORE IDEA: Same as convex scanline fill — process one row at
// a time and fill between edge crossings. But for a concave
// (non-convex) polygon, a single horizontal row can cross the
// boundary MORE THAN TWICE (e.g. a star or L-shape has 4+ crossings).
// The simple xmin/xmax approach breaks down.
//
// This algorithm uses the classic EDGE TABLE + ACTIVE EDGE LIST (AET):
//
//   EDGE TABLE (ET): One bucket per y-row. Each bucket holds all
//   polygon edges that START at that y value (their lower endpoint).
//   Each entry stores: starting x, 1/slope (Δx per row), and ymax.
//
//   ACTIVE EDGE LIST (AEL): The set of edges currently "active"
//   (crossing the current scanline). At each new y row:
//     1. Move edges from ET into AEL that start at this y.
//     2. Remove edges from AEL that ended before this y.
//     3. Sort AEL by current x value.
//     4. Fill pixel pairs: between AEL[0] and AEL[1], AEL[2] and AEL[3], etc.
//     5. Advance each edge's x by its 1/slope for the next row.
//
// This correctly handles any polygon, even with holes or concavities.
// ============================================================

#include "non_convex_filling_algorithm.h" // Header declaring this class
#include "convex_filling_algorithm.h"     // Not directly used but included for consistency
#include <climits>                        // INT_MAX — used as sentinel value
#include <vector>                         // std::vector — for the edge table array
#include <cmath>                          // std::round, std::ceil, std::floor
#include <algorithm>                      // std::min, std::swap
#include <list>                           // std::list — for the Active Edge List (efficient insert/erase mid-list)

// ── File-local helpers ────────────────────────────────────────
namespace {

    // EdgeRec: one entry in the edge table, describing one polygon edge
    struct EdgeRec {
        double x;    // Current x-intercept of this edge at the current scanline (updated each row)
        double minv; // Δx per row = (x2 - x1) / (y2 - y1) — the "inverse slope"
                     // Added to x each time we advance one row down
        int ymax;    // The y-row where this edge ENDS (top vertex y in screen coords)
                     // When the scanline y reaches ymax, remove this edge from the AEL

        // Comparison operator so we can sort the AEL by current x values
        bool operator<(const EdgeRec& r) const {
            return x < r.x; // Smaller x = further left = comes first in sorted order
        }
    };

    typedef std::list<EdgeRec> EdgeList; // An AEL (Active Edge List) is a linked list of EdgeRec

    // Build one EdgeRec from two consecutive polygon vertices v1 and v2
    EdgeRec InitEdgeRec(Point v1, Point v2) {
        if (v1.y > v2.y) std::swap(v1, v2); // Ensure v1 is the top vertex (lower y value)
        EdgeRec rec;
        rec.x    = v1.x;                                    // Starting x = top vertex's x
        rec.ymax = (int)std::round(v2.y);                   // End row = bottom vertex's y (rounded to integer)
        rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);  // Δx/Δy = inverse slope of the edge
        return rec;
    }

    // Populate the edge table from the polygon vertices.
    // Each edge is inserted into the bucket corresponding to its starting (top) y row.
    void InitEdgeTable(const std::vector<Point>& polygon, std::vector<EdgeList>& table, int minY) {
        if (polygon.empty()) return;

        Point v1 = polygon.back(); // Start the edge loop from the last vertex (connecting last to first)
        for (size_t i = 0; i < polygon.size(); i++) {
            Point v2 = polygon[i];

            int y1 = (int)std::round(v1.y);
            int y2 = (int)std::round(v2.y);

            if (y1 == y2) { // Horizontal edge — skipped (contributes no scanline crossing)
                v1 = v2;
                continue;
            }

            EdgeRec rec = InitEdgeRec(v1, v2); // Build the edge record for this edge

            int startY = (int)std::round(std::min(v1.y, v2.y)); // The y row where this edge starts (its top/lowest y)
            int idx    = startY - minY; // Convert absolute y to table index (table[0] = row minY)

            if (idx >= 0 && idx < (int)table.size()) {
                table[idx].push_back(rec); // Insert this edge into the appropriate bucket
            }

            v1 = polygon[i]; // Advance: current vertex becomes the previous for the next edge
        }
    }
}

// Constructor
NonConvexFillingAlgorithm::NonConvexFillingAlgorithm() : FillingAlgorithm("NonConvexFillingAlgorithm") {

}

// ── Scanline fill with AET ────────────────────────────────────
void NonConvexFillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    std::vector<Point> pts = shape.getSidePoints(); // Get the polygon's boundary vertices

    if (pts.empty()) {
        pts = shape.points; // Fallback to raw defining points if no pre-computed side points exist
    }
    if (pts.size() < 3) return; // Need at least a triangle to fill

    // ── STEP 1: Find the vertical extent of the polygon ───────
    int minY =  INT_MAX;
    int maxY = -INT_MAX;
    for (const auto& p : pts) {
        if ((int)std::round(p.y) < minY) minY = (int)std::round(p.y);
        if ((int)std::round(p.y) > maxY) maxY = (int)std::round(p.y);
    }

    if (minY > maxY) return; // Degenerate shape

    int numEntries = maxY - minY + 2; // Table height (+2 safety margin)
    if (numEntries <= 0 || numEntries > 10000) return; // Sanity bounds

    // ── STEP 2: Build the Edge Table ──────────────────────────
    std::vector<EdgeList> table(numEntries); // One EdgeList bucket per scanline row
    InitEdgeTable(pts, table, minY);         // Fill in all edge records

    // ── STEP 3: Find the first non-empty row ──────────────────
    int y = 0;
    while(y < numEntries && table[y].empty()) {
        y++; // Skip rows that have no edges starting there
    }
    if(y == numEntries) return; // No edges found — nothing to fill

    // ── STEP 4: Initialize the Active Edge List ───────────────
    EdgeList ActiveList = table[y]; // Start with edges that begin at the first non-empty row
    COLORREF color = shape.fillColor;

    // ── STEP 5: Sweep downward through all rows ───────────────
    while (!ActiveList.empty()) { // Continue until no more active edges remain

        ActiveList.sort(); // Sort AEL by current x value — required for correct left/right pairing below

        // ── Fill pixel pairs between consecutive AEL entries ──
        // Rule: fill BETWEEN edge crossings, not outside them.
        // Pairs: [AEL[0]→AEL[1]], [AEL[2]→AEL[3]], etc.
        // This correctly handles concave polygons with multiple crossings per row.
        for(auto it = ActiveList.begin(); it != ActiveList.end(); ) {
            int x1 = (int)std::ceil(it->x); // Left edge of this fill span (ceil = stay inside)
            it++;                            // Advance to the right boundary of this span
            if (it == ActiveList.end()) break; // Odd number of crossings — shouldn't happen in valid polygon, but guard anyway

            int x2 = (int)std::floor(it->x); // Right edge of this fill span (floor = stay inside)
            it++;                             // Advance past this pair to the start of the next pair

            int actualY = y + minY; // Convert table index back to absolute screen y coordinate

            for(int x = x1; x <= x2; x++) { // Fill every pixel in this horizontal span
                if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, actualY))) {
                    sw->setPixel(x, actualY, color); // Paint pixel, respecting any clipping region
                }
            }
        }

        // ── Advance to the next row ────────────────────────────
        y++;
        int actualY = y + minY;

        // Remove edges from the AEL that have reached their top vertex (ymax)
        for(auto it = ActiveList.begin(); it != ActiveList.end(); ) {
            if(actualY == it->ymax) {
                it = ActiveList.erase(it); // Erase returns the iterator to the next element
            } else {
                it++; // This edge is still active — keep it
            }
        }

        // Update each remaining active edge's x by its inverse slope
        for(auto& edge : ActiveList) {
            edge.x += edge.minv; // Move x along the edge by one row's worth: x += Δx/Δy
        }

        // Add new edges that start at the next row (move from ET bucket to AEL)
        if (y < numEntries) {
            ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
        }
    }
}

// ── Public entry point ────────────────────────────────────────
void NonConvexFillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (&clippingRegion != nullptr && !clippingRegion.isInside(startPoint)) {
        return; // Start point is outside the clipping region — abort
    }
    sw->activate();
    fill_helper(shape, clippingRegion, startPoint, sw);
    sw->deactivate();
}
