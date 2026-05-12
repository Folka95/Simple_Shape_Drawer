#ifndef CIRCLE_H
#define CIRCLE_H
#include "../shape.h"


class Circle : public Shape {
private:
    double radius;
    Point center;
public:
    Circle();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    int scanQuarter(const Point &point) const;
    double getRadius() const;
    Point getCenter() const;
    Shape* clone() const override;
};



#endif
