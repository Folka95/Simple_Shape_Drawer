#pragma once
#ifndef SAD_SMILE_FACE_DRAWING_ALGORITHM_H
#define SAD_SMILE_FACE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/sad_smile_face.h"


class SadSmileFace_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void drawCircle(SadSmileFace* face, const Point& center, double radius, ScreenWriter *sw) const ;
    void drawNose(SadSmileFace* face, double radius, ScreenWriter *sw) const ;
    void drawMouth(SadSmileFace* face, double radius, ScreenWriter *sw) const ;
    void runAlgorithm(SadSmileFace* face, ScreenWriter *sw) const;
public:
    SadSmileFace_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif //SAD_SMILE_FACE_DRAWING_ALGORITHM_H
