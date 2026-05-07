#ifndef CIRCLE_H
#define CIRCLE_H
#include "../shape.h"


class Circle : public Shape {
public:
    Circle();
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
};



#endif
