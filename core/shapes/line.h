#pragma once
#ifndef LINE_H
#define LINE_H
#include "../shape.h"


class Line : public Shape {
public:
    Line();
    void initialize() override;
    bool isInside(const Point &point) const ;
    bool isEnoughToDraw() const ;
    void takeAction(int actionID) override;
    Shape* clone() const ;
};



#endif
