#include "square_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/line.h"

Square_DrawingAlgorithm::Square_DrawingAlgorithm() : DrawingAlgorithm("Square_DrawingAlgorithm") {}

void Square_DrawingAlgorithm::drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const{
    Line* line = new Line();
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm* line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}


void Square_DrawingAlgorithm::runAlgorithm(Square* square, ScreenWriter *sw) const {
    drawLine(square->getTopLeft(), square->getTopRight(), sw);
    drawLine(square->getTopRight(), square->getBottomRight(), sw);
    drawLine(square->getBottomRight(), square->getBottomLeft(), sw);
    drawLine(square->getTopLeft(), square->getBottomLeft(), sw);
}

void Square_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_SQUARE){
        std::cerr << "Square_DrawingAlgorithm::draw : shape to draw must be Square" << std::endl;
        return;
    }
    if(!inputShape.isEnoughToDraw()){
        std::cerr << "Square_DrawingAlgorithm::draw : shape is not enough to draw" << std::endl;
        return;
    }
    Square* square = dynamic_cast<Square*>(inputShape.clone());
    this->runAlgorithm(square, sw);
}






