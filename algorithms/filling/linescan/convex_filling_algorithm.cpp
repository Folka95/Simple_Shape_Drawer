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

ConvexFillingAlgorithm::ConvexFillingAlgorithm() : FillingAlgorithm() {

}

void ConvexFillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    sw->activate();
    fill_helper(shape, clippingRegion, startPoint, sw);
    sw->deactivate();
}

void ConvexFillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    std::vector<Point> pts = shape.getSidePoints();
    
    // Fallback: If the shape doesn't inherently provide boundary points (like Circle/Ellipse)
    if (pts.empty() && !shape.points.empty()) {
        const double PI = std::acos(-1.0);
        if ((shape.getType() == SHAPE_CIRCLE || shape.getType() == SHAPE_SAD_SMILE_FACE || shape.getType() == SHAPE_HAPPY_SMILE_FACE) && shape.points.size() >= 2) {
            double radius = shape.points[0].euclideanDistance(shape.points[1]);
            Point center = shape.points[0];
            int num_segments = 360; 
            for(int i = 0; i < num_segments; i++) {
                double theta = 2.0 * PI * double(i) / double(num_segments);
                pts.push_back(Point(center.x + radius * cos(theta), center.y + radius * sin(theta)));
            }
        } else if (shape.getType() == SHAPE_ELLIPSE && shape.points.size() >= 3) {
            Point center = shape.points[0];
            double dx1 = std::abs(shape.points[1].x - center.x);
            double dy1 = std::abs(shape.points[1].y - center.y);
            double dx2 = std::abs(shape.points[2].x - center.x);
            double dy2 = std::abs(shape.points[2].y - center.y);
            double radius1 = std::max(dx1, dx2);
            double radius2 = std::max(dy1, dy2);
            int num_segments = 360; 
            for(int i = 0; i < num_segments; i++) {
                double theta = 2.0 * PI * double(i) / double(num_segments);
                pts.push_back(Point(center.x + radius1 * cos(theta), center.y + radius2 * sin(theta)));
            }
        } else {
            pts = shape.points;
        }
    }

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
                sw->setPixel(x, y, color);
            }
        }
    }
}