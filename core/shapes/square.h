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
    Shape* clone() const override;
    Point getTopLeft() const ;
    Point getTopRight() const ;
    Point getBottomLeft() const ;
    Point getBottomRight() const ;
    double getSideLength() const ;
    std::vector< Point > getSidePoints() const override;
};



#endif //SQUARE_H
