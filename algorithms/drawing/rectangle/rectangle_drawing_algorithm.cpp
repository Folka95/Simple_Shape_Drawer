#include "rectangle_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"

Rectangle_DrawingAlgorithm::Rectangle_DrawingAlgorithm() : DrawingAlgorithm("Rectangle_DrawingAlgorithm"){}

void Rectangle_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const{
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
        DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
        line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Rectangle_DrawingAlgorithm::runAlgorithm(RectangleShape* rectangle, ScreenWriter *sw) const {
    drawLine(rectangle->getTopLeft(), rectangle->getTopRight(), sw);
    drawLine(rectangle->getTopRight(), rectangle->getBottomRight(), sw);
    drawLine(rectangle->getBottomRight(), rectangle->getBottomLeft(), sw);
    drawLine(rectangle->getTopLeft(), rectangle->getBottomLeft(), sw);
}

void Rectangle_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_RECTANGLE){
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape to draw must be Rectangle" << std::endl;
        return;
    }
    if(!inputShape.isEnoughToDraw()){
        std::cerr << "Rectangle_DrawingAlgorithm::draw : shape is not enough to draw" << std::endl;
        return;
    }
    RectangleShape* rectangle = dynamic_cast<RectangleShape*>(inputShape.clone());
    this->runAlgorithm(rectangle, sw);
}






