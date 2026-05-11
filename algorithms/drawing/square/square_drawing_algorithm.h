#ifndef SQUARE_DRAWING_ALGORITHM_H
#define SQUARE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"


class Square_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void drawLine(const Point& p1,const Point& p2, ScreenWriter *sw) const;
public:
    Square_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif
