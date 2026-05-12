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
    Shape* clone() const override;
    std::vector< Point > getSidePoints() const ;
    double getRadius() const ;
    Point getCenter() const ;
    int scanQuarter(const Point &point) const;
};



#endif
