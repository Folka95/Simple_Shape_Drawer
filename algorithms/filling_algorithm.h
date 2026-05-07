#pragma once
#ifndef FILLINGALGORITHM_H
#define FILLINGALGORITHM_H
#include "../core/shape.h"
#include "../rendering/screen_writer.h"

class FillingAlgorithm {
protected:
    FillingAlgorithm() { }
public:
    virtual void fill(const Shape &shape, const Shape &clippingRegion, ScreenWriter *sw) const = 0;
};

#endif
