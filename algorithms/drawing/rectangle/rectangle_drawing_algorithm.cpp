//
// Created by frasa on 5/11/2026.

#include "rectangle_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"

Rectangle_DrawingAlgorithm::Rectangle_DrawingAlgorithm() : DrawingAlgorithm(){}

void Rectangle_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const{
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
        DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
        line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Rectangle_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_RECTANGLE){
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape to draw must be Rectangle" << std::endl;
        return;
    }
    if(shape.points.size() != 2){
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape must have 2 points" << std::endl;
        return;
    }
    Point topLeft(shape.points[0]);
    Point bottomRight(shape.points[1]);
    Point topRight(bottomRight.x, topLeft.y);
    Point bottomLeft(topLeft.x, bottomRight.y);
    drawLine(topLeft, topRight, sw);
    drawLine(topRight, bottomRight, sw);
    drawLine(bottomRight, bottomLeft, sw);
    drawLine(topLeft, bottomLeft, sw);
}





