#pragma once
#ifndef DRAWINGALGORITH_H
#define DRAWINGALGORITH_H
#include "../core/shape.h"
#include <string>
#include "../rendering/screen_writer.h"

class DrawingAlgorithm {
protected:
    std::string name;
    DrawingAlgorithm(std::string name) : name(name) { }
    
public:
    std::string getName() const { return name; }
    virtual void draw(const Shape &shape, ScreenWriter *sw) const = 0;
};

#endif
