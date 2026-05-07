#ifndef CIRCLE_H
#define CIRCLE_H
#include "../shape.h"


class Circle : public Shape {
private:
    double radius;
public:
    Circle();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
};



#endif
