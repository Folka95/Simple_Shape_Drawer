#pragma once
#ifndef DRAWINGALGORITH_H
#define DRAWINGALGORITH_H
#include "../core/shape.h"

class DrawingAlgorithm {
private:
    DrawingAlgorithm() { }
public:
    virtual void draw(const Shape &shape) = 0;
};

#endif
