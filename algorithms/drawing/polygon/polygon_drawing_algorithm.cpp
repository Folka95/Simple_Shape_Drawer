#pragma once
#include "polygon_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"
#include <algorithm>

Polygon_DrawingAlgorithm::Polygon_DrawingAlgorithm() : DrawingAlgorithm() {}

void Polygon_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const {
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Polygon_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_POLYGON){
        std::cerr << "Polygon_DrawingAlgorithm::draw : shape to draw must be Rectangle" << std::endl;
        return;
    }
    if (shape.getSize() == 0){
        std::cerr << "Polygon_DrawingAlgorithm::draw : polygon has zero points" << std::endl;
        return;
    }
    Shape *tmp = shape.clone();
    sort(tmp->points.begin(), tmp->points.end());
    for(int i = 0; i < tmp->points.size(); i++) {
        drawLine(tmp->points[i], tmp->points[(i + 1) % tmp->points.size()], sw);
    }
}





