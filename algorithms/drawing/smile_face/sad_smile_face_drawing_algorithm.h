#ifndef SAD_SMILE_FACE_DRAWING_ALGORITHM_H
#define SAD_SMILE_FACE_DRAWING_ALGORITHM_H
#include "../../drawing_algorithm.h"
#include "../../../core/shapes/sad_smile_face.h"


class SadSmileFace_DrawingAlgorithm : public DrawingAlgorithm {
private:
    void drawCircle(const Shape &face, const Point& center, double radius, ScreenWriter *sw) const ;
    void drawNose(const Shape &face, double radius, ScreenWriter *sw) const ;
    void drawMouth(const Shape &face, double radius, ScreenWriter *sw) const ;
public:
    SadSmileFace_DrawingAlgorithm();
    void draw(const Shape &shape, ScreenWriter *sw) const;
};



#endif //SAD_SMILE_FACE_DRAWING_ALGORITHM_H
