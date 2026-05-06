#pragma once
#ifndef LINE_H
#define LINE_H
#include "../shape.h"


class Line : public Shape {
public:
    Line();
    bool isEnoughToDraw() const ;
    Shape* clone() const ;
};



#endif
