#include "convex_filling_algorithm.h"

namespace {
    struct Entry {
        int xmin, xmax;
    };

    void InitEntries(std::vector<Entry>& table) {
        for(size_t i = 0; i < table.size(); i++) {
            table[i].xmin = INT_MAX;
            table[i].xmax = -INT_MAX;
        }
    }

    void ScanEdge(Point v1, Point v2, std::vector<Entry>& table, int minY) {
        if((int)v1.y == (int)v2.y) return;
        if(v1.y > v2.y) std::swap(v1, v2);
        
        double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
        double x = v1.x;
        int y = (int)v1.y;
        int endY = (int)v2.y;
        
        while(y < endY) {
            int idx = y - minY;
            if (idx >= 0 && idx < (int)table.size()) {
                if(x < table[idx].xmin) table[idx].xmin = (int)std::ceil(x);
                if(x > table[idx].xmax) table[idx].xmax = (int)std::floor(x);
            }
            y++;
            x += minv;
        }
    }
}

ConvexFillingAlgorithm::ConvexFillingAlgorithm() : FillingAlgorithm("ConvexFillingAlgorithm") {

}

void ConvexFillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    if (&clippingRegion != nullptr && !clippingRegion.isInside(startPoint)) {
        return;
    }
    sw->activate();
    fill_helper(shape, clippingRegion, startPoint, sw);
    sw->deactivate();
}

void ConvexFillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    std::vector<Point> pts = shape.getSidePoints();
    if (pts.empty() && !shape.points.empty()) {
        pts = shape.points;
    }
    
    if (pts.size() < 3) return;

    // 1. Find min and max Y to construct dynamic table size
    int minY = INT_MAX;
    int maxY = -INT_MAX;
    for (const auto& p : pts) {
        if ((int)p.y < minY) minY = (int)p.y;
        if ((int)p.y > maxY) maxY = (int)p.y;
    }
    
    if (minY > maxY) return;
    
    int numEntries = maxY - minY + 2; 
    if (numEntries <= 0 || numEntries > 10000) return; // sanity bounds
    
    // 2. Initialize Entry Table
    std::vector<Entry> table(numEntries);
    InitEntries(table);
    
    // 3. Scan all edges
    Point v1 = pts.back();
    for(size_t i = 0; i < pts.size(); i++) {
        Point v2 = pts[i];
        ScanEdge(v1, v2, table, minY);
        v1 = pts[i];
    }
    
    // 4. Draw horizontal scanlines
    COLORREF color = shape.fillColor;
    for(int i = 0; i < numEntries; i++) {
        if(table[i].xmin <= table[i].xmax) {
            int y = i + minY;
            for(int x = table[i].xmin; x <= table[i].xmax; x++) {
                if (&clippingRegion == nullptr || clippingRegion.isInside(Point(x, y))) {
                    sw->setPixel(x, y, color);
                }
            }
        }
    }
}