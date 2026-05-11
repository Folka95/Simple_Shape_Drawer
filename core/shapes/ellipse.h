#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "../shape.h"

class EllipseShape : public Shape {
private:
    double radius1;
    double radius2;
public:
    EllipseShape();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
};

#endif
