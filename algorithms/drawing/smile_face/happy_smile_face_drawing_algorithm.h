#ifndef HAPPY_SMILE_FACE_DRAWING_ALGORITHM_H
#define HAPPY_SMILE_FACE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/happy_smile_face.h"

class HappySmileFace_DrawingAlgorithm: public DrawingAlgorithm{
private:
    void drawCircle(const Shape &face, const Point& center, double radius, ScreenWriter *sw) const ;
    void drawNose(const Shape &face, double radius, ScreenWriter *sw) const ;
    void drawMouth(const Shape &face, double radius, ScreenWriter *sw) const ;
public:
    HappySmileFace_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};

#endif //HAPPY_SMILE_FACE_DRAWING_ALGORITHM_H
