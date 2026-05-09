#ifndef CURVE_H
#define CURVE_H
#include "../shape.h"


class CurveShape : public Shape {
public:
    CurveShape();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
};



#endif //CURVE_H
