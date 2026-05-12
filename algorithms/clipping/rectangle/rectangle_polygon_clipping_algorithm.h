#pragma once
#ifndef SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H
#define SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H

#include "../../clipping_algorithm.h"
#include "../../../core/shapes/rectangle.h"
#include <vector>




class Rectangle_Polygon_ClippingAlgorithm : public ClippingAlgorithm {
private:
    Point intersectVertical(const Point &a, const Point &b, double xLine) const;
    Point intersectHorizontal(const Point &a, const Point &b, double yLine) const;
    std::vector<Point> clipAgainstLeft(const std::vector<Point> &input, double xmin) const;
    std::vector<Point> clipAgainstRight(const std::vector<Point> &input, double xmax) const;
    std::vector<Point> clipAgainstTop(const std::vector<Point> &input, double ymin) const;
    std::vector<Point> clipAgainstBottom(const std::vector<Point> &input, double ymax) const;
    std::vector<Point> sutherlandHodgman(const std::vector<Point> &subject, double xmin, double xmax, double ymin,double ymax) const;
        void drawPolygonOutline(const std::vector<Point> &vertices, COLORREF borderColor, ScreenWriter *sw) const;

    void runAlgorithm(Shape *polygon, RectangleShape *rectangle, ScreenWriter *sw) const;

public:
    Rectangle_Polygon_ClippingAlgorithm();

    void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const;
};

#endif //SIMPLE_SHAPE_DRAWER_RECTANGLE_POLYGON_CLIPPING_ALGORITHM_H
