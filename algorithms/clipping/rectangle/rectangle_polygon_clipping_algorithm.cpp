#include "rectangle_polygon_clipping_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"
#include <algorithm>
#include <cmath>
#include <iostream>


constexpr double kEps = 1e-9;

Rectangle_Polygon_ClippingAlgorithm::Rectangle_Polygon_ClippingAlgorithm() : ClippingAlgorithm() {}


Point Rectangle_Polygon_ClippingAlgorithm::intersectVertical(const Point &a, const Point &b, double xLine) const{
    Point res{xLine, a.y};
    const double dx = b.x - a.x;
    if (std::fabs(dx) > kEps) {
        const double t = (xLine - a.x) / dx;
        res.y = a.y + t * (b.y - a.y);
    }
    return res;
}

Point Rectangle_Polygon_ClippingAlgorithm::intersectHorizontal(const Point &a, const Point &b, double yLine)const {
    Point res{a.x, yLine};
    const double dy = b.y - a.y;
    if (std::fabs(dy) > kEps) {
        const double t = (yLine - a.y) / dy;
        res.x = a.x + t * (b.x - a.x);
    }
    return res;
}

std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstLeft(const std::vector<Point> &input, double xmin) const{
    std::vector<Point> output;
    if (input.empty()) {
        return output;
    }
    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.x >= xmin - kEps;
        const bool eIn = e.x >= xmin - kEps;
        if (eIn) {
            if (!sIn) {
                output.push_back(Rectangle_Polygon_ClippingAlgorithm::intersectVertical(s, e, xmin));
            }
            output.push_back(e);
        } else if (sIn) {
            output.push_back(Rectangle_Polygon_ClippingAlgorithm::intersectVertical(s, e, xmin));
        }
        s = e;
    }
    return output;
}

std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstRight(const std::vector<Point> &input, double xmax)const {
    std::vector<Point> output;
    if (input.empty()) {
        return output;
    }
    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.x <= xmax + kEps;
        const bool eIn = e.x <= xmax + kEps;
        if (eIn) {
            if (!sIn) {
                output.push_back(intersectVertical(s, e, xmax));
            }
            output.push_back(e);
        } else if (sIn) {
            output.push_back(intersectVertical(s, e, xmax));
        }
        s = e;
    }
    return output;
}

std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstTop(const std::vector<Point> &input, double ymin) const{
    std::vector<Point> output;
    if (input.empty()) {
        return output;
    }
    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.y >= ymin - kEps;
        const bool eIn = e.y >= ymin - kEps;
        if (eIn) {
            if (!sIn) {
                output.push_back(Rectangle_Polygon_ClippingAlgorithm::intersectHorizontal(s, e, ymin));
            }
            output.push_back(e);
        } else if (sIn) {
            output.push_back(Rectangle_Polygon_ClippingAlgorithm::intersectHorizontal(s, e, ymin));
        }
        s = e;
    }
    return output;
}

std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::clipAgainstBottom(const std::vector<Point> &input, double ymax) const{
    std::vector<Point> output;
    if (input.empty()) {
        return output;
    }
    Point s = input.back();
    for (const Point &e : input) {
        const bool sIn = s.y <= ymax + kEps;
        const bool eIn = e.y <= ymax + kEps;
        if (eIn) {
            if (!sIn) {
                output.push_back(intersectHorizontal(s, e, ymax));
            }
            output.push_back(e);
        } else if (sIn) {
            output.push_back(intersectHorizontal(s, e, ymax));
        }
        s = e;
    }
    return output;
}

std::vector<Point> Rectangle_Polygon_ClippingAlgorithm::sutherlandHodgman(const std::vector<Point> &subject, double xmin, double xmax, double ymin,double ymax) const{
    std::vector<Point> work = clipAgainstLeft(subject, xmin);
    work = clipAgainstRight(work, xmax);
    work = clipAgainstBottom(work, ymax);
    work = clipAgainstTop(work, ymin);
    return work;
}




void Rectangle_Polygon_ClippingAlgorithm::drawPolygonOutline(const std::vector<Point> &vertices, COLORREF borderColor,
                                                           ScreenWriter *sw) const {
    const int n = static_cast<int>(vertices.size());
    if (n < 2) {
        return;
    }
    if (n == 2) {
        Line *line = new Line();
        line->borderColor = borderColor;
        line->addPoint(vertices[0]);
        line->addPoint(vertices[1]);
        DrawingAlgorithm *drawer = new Line_Midpoint_DrawingAlgorithm();
        drawer->draw(*line, sw);
        return;
    }
    for (int i = 0; i < n; ++i) {
        Line *line = new Line();
        line->borderColor = borderColor;
        line->addPoint(vertices[i]);
        line->addPoint(vertices[(i + 1) % n]);
        DrawingAlgorithm *drawer = new Line_Midpoint_DrawingAlgorithm();
        drawer->draw(*line, sw);
    }
}

void Rectangle_Polygon_ClippingAlgorithm::runAlgorithm(Shape *polygon, RectangleShape *rectangle, ScreenWriter *sw) const {
    if (polygon == nullptr || rectangle == nullptr || sw == nullptr) {
        return;
    }
    if (polygon->points.size() < 2 || rectangle->points.size() < 2) {
        return;
    }
    const double xmin = rectangle->getTopLeft().x;
    const double xmax = rectangle->getTopRight().x;
    const double ymin = rectangle->getTopRight().y;
    const double ymax = rectangle->getBottomLeft().y;

    const std::vector<Point> clipped = sutherlandHodgman(polygon->getSidePoints(), xmin, xmax, ymin, ymax);
    if (clipped.size() < 2) {
        return;
    }
    drawPolygonOutline(clipped, polygon->borderColor, sw);
}

void Rectangle_Polygon_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON) {
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : shape to draw must be Polygon" << std::endl;
        return;
    }

    if (inputRegion.getType() != SHAPE_RECTANGLE) {
        std::cerr << "Rectangle_Polygon_ClippingAlgorithm::clip : Region must be a Rectangle" << std::endl;
        return;
    }

    Shape *polygon = inputShape.clone();
    RectangleShape *rectangle = dynamic_cast<RectangleShape *>(inputRegion.clone());
    if (rectangle == nullptr) {
        delete polygon;
        return;
    }
    this->runAlgorithm(polygon, rectangle, sw);
    delete polygon;
    delete rectangle;
}
