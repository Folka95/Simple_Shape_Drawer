#ifndef SAD_SMILE_FACE_H
#define SAD_SMILE_FACE_H
#include "../shape.h"


class SadSmileFace : public Shape {
private:
    double radius;
public:
    SadSmileFace();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
    double getRadius() const;
    std::vector< Point > getSidePoints() const override;
};



#endif
