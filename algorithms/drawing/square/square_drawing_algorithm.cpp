#include "square_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"

Square_DrawingAlgorithm::Square_DrawingAlgorithm() : DrawingAlgorithm() {}

void Square_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const{
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Square_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if (shape.getType() != SHAPE_SQUARE){
        std::cerr << "Square_DrawingAlgorithm::draw : shape to draw must be Square" << std::endl;
        return;
    }
    if(!shape.isEnoughToDraw()){
        std::cerr << "Square_DrawingAlgorithm::draw : shape is not enough to draw" << std::endl;
        return;
    }
    // TODO
    Point topLeft;
    Point bottomRight;
    Point topRight;
    Point bottomLeft;

    drawLine(topLeft, topRight, sw);
    drawLine(topRight, bottomRight, sw);
    drawLine(bottomRight, bottomLeft, sw);
    drawLine(topLeft, bottomLeft, sw);
}





