#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "../shape.h"


class RectangleShape : public Shape  {
private:
    double width;
    double height;
public:
    RectangleShape();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
};



#endif //RECTANGLE_H
