#ifndef HAPPY_SMILE_FACE_H
#define HAPPY_SMILE_FACE_H
#include "../shape.h"


class HappySmileFace : public Shape {
private:
    double radius;
public:
    HappySmileFace();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
    double getRadius() const;
    std::vector< Point > getSidePoints() const override;
};



#endif
