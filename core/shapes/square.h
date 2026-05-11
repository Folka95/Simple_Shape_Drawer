#ifndef SQUARE_H
#define SQUARE_H
#include "../shape.h"


class Square : public Shape {
private:
    double sideLength;
public:
    Square();
    void initialize() override;
    bool isInside(const Point &point) const override;
    bool isEnoughToDraw() const override;
    void takeAction(int actionID) override;
    Shape* clone() const override;
};



#endif //SQUARE_H
