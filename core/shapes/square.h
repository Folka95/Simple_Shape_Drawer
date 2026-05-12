#ifndef SQUARE_H
#define SQUARE_H
#include "../shape.h"


class Square : public Shape {
private:
    double sideLength;
    Point topLeft, topRight, bottomLeft, bottomRight;
public:
    Square();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
    Point getTopLeft() const ;
    Point getTopRight() const ;
    Point getBottomLeft() const ;
    Point getBottomRight() const ;
    double getSideLength() const ;
};



#endif //SQUARE_H
