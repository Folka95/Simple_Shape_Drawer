#include "polygon_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"
#include <algorithm>

Polygon_DrawingAlgorithm::Polygon_DrawingAlgorithm() : DrawingAlgorithm("Polygon_DrawingAlgorithm") {}

void Polygon_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, COLORREF color, ScreenWriter *sw) const {
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    line->borderColor = color;
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Polygon_DrawingAlgorithm::runAlgorithm(Shape* polygon, ScreenWriter *sw) const {
    vector< Point > points = polygon->getSidePoints();
    for(int i = 0; i < polygon->points.size(); i++) {
        drawLine(points[i], points[(i + 1) % points.size()], polygon->borderColor, sw);
    }
}


void Polygon_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_POLYGON){
        std::cerr << "Polygon_DrawingAlgorithm::draw : shape to draw must be Polygon" << std::endl;
        return;
    }
    if (inputShape.getSize() == 0){
        std::cerr << "Polygon_DrawingAlgorithm::draw : polygon has zero points" << std::endl;
        return;
    }
    this->runAlgorithm(inputShape.clone(), sw);
}






