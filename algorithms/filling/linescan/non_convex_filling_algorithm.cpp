#include "non_convex_filling_algorithm.h"
#include "convex_filling_algorithm.h"
#include <climits>
#include <vector>
#include <cmath>
#include <algorithm>
#include <list>

namespace {
    struct EdgeRec {
        double x;
        double minv;
        int ymax;
        bool operator<(const EdgeRec& r) const {
            return x < r.x;
        }
    };
    
    typedef std::list<EdgeRec> EdgeList;

    EdgeRec InitEdgeRec(Point v1, Point v2) {
        if (v1.y > v2.y) std::swap(v1, v2);
        EdgeRec rec;
        rec.x = v1.x;
        rec.ymax = (int)std::round(v2.y);
        rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
        return rec;
    }

    void InitEdgeTable(const std::vector<Point>& polygon, std::vector<EdgeList>& table, int minY) {
        if (polygon.empty()) return;
        Point v1 = polygon.back();
        for (size_t i = 0; i < polygon.size(); i++) {
            Point v2 = polygon[i];
            int y1 = (int)std::round(v1.y);
            int y2 = (int)std::round(v2.y);
            if (y1 == y2) {
                v1 = v2;
                continue;
            }
            EdgeRec rec = InitEdgeRec(v1, v2);
            int startY = (int)std::round(std::min(v1.y, v2.y));
            int idx = startY - minY;
            if (idx >= 0 && idx < (int)table.size()) {
                table[idx].push_back(rec);
            }
            v1 = polygon[i];
        }
    }
}

NonConvexFillingAlgorithm::NonConvexFillingAlgorithm() : FillingAlgorithm() {

}

void NonConvexFillingAlgorithm::fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    std::vector<Point> pts = shape.getSidePoints();
    if (pts.empty()) {
        pts = shape.points;
    }
    if (pts.size() < 3) return;

    // 1. Find min and max Y to construct dynamic table size
    int minY = INT_MAX;
    int maxY = -INT_MAX;
    for (const auto& p : pts) {
        if ((int)std::round(p.y) < minY) minY = (int)std::round(p.y);
        if ((int)std::round(p.y) > maxY) maxY = (int)std::round(p.y);
    }
    
    if (minY > maxY) return;
    
    int numEntries = maxY - minY + 2; 
    if (numEntries <= 0 || numEntries > 10000) return; // sanity bounds
    
    std::vector<EdgeList> table(numEntries);
    InitEdgeTable(pts, table, minY);
    
    int y = 0;
    while(y < numEntries && table[y].empty()) {
        y++;
    }
    if(y == numEntries) return;
    
    EdgeList ActiveList = table[y];
    COLORREF color = shape.fillColor;
    
    while (!ActiveList.empty()) {
        ActiveList.sort();
        
        for(auto it = ActiveList.begin(); it != ActiveList.end(); ) {
            int x1 = (int)std::ceil(it->x);
            it++;
            if (it == ActiveList.end()) break; // Prevent out of bounds if odd edges
            int x2 = (int)std::floor(it->x);
            it++;
            
            int actualY = y + minY;
            for(int x = x1; x <= x2; x++) {
                sw->setPixel(x, actualY, color);
            }
        }
        
        y++;
        int actualY = y + minY;
        
        for(auto it = ActiveList.begin(); it != ActiveList.end(); ) {
            if(actualY == it->ymax) {
                it = ActiveList.erase(it);
            } else {
                it++;
            }
        }
        
        for(auto& edge : ActiveList) {
            edge.x += edge.minv;
        }
        
        if (y < numEntries) {
            ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
        }
    }
}

void NonConvexFillingAlgorithm::fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const {
    sw->activate();
    fill_helper(shape, clippingRegion, startPoint, sw);
    sw->deactivate();
}