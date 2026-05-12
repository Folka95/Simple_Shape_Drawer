#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "../shape.h"


class RectangleShape : public Shape  {
private:
    double width;
    double height;
    Point topLeft, topRight, bottomLeft, bottomRight;
public:
    RectangleShape();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
    Point getTopLeft() const ;
    Point getTopRight() const ;
    Point getBottomLeft() const ;
    Point getBottomRight() const ;
    double getWidth() const ;
    double getHeight() const ;
};



#endif //RECTANGLE_H
