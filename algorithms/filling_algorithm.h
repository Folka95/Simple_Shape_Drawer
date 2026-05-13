#pragma once
#ifndef FILLINGALGORITHM_H
#define FILLINGALGORITHM_H
#include "../core/shape.h"
#include <string>
#include "../rendering/screen_writer.h"

class FillingAlgorithm {
protected:
    std::string name;
    FillingAlgorithm(std::string name) : name(name) { }
    
public:
    std::string getName() const { return name; }
    virtual void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const = 0;
};

#endif
