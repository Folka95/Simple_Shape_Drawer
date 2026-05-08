#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "../shape.h"


class Rectangle : public Shape  {
private:
    double width;
    double height;
public:
    Rectangle();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    Shape* clone() const override;
};



#endif //RECTANGLE_H
