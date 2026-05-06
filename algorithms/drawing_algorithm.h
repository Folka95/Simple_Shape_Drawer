#pragma once
#ifndef DRAWINGALGORITH_H
#define DRAWINGALGORITH_H
#include "../core/shape.h"
#include "../rendering/screen_writer.h"

class DrawingAlgorithm {
protected:
    DrawingAlgorithm() { }
public:
    virtual void draw(const Shape &shape, ScreenWriter *sw) const = 0;
};

#endif
