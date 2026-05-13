#pragma once
#ifndef CLIPPING_ALGORITHM_H
#define CLIPPING_ALGORITHM_H
#include "drawing_algorithm.h"
#include "../core/shape.h"
#include <string>
#include "../rendering/screen_writer.h"

class ClippingAlgorithm {
protected:
    std::string name;
    ClippingAlgorithm(std::string name) : name(name) { }
    
public:
    std::string getName() const { return name; }
    virtual void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const = 0;
};

#endif //CLIPPING_ALGORITHM_H
