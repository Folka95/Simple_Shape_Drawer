#pragma once
#ifndef CLIPPING_ALGORITHM_H
#define CLIPPING_ALGORITHM_H
#include "drawing_algorithm.h"
#include "../core/shape.h"
#include "../rendering/screen_writer.h"

class ClippingAlgorithm {
protected:
    ClippingAlgorithm() { }
public:
    virtual void clip(const Shape &shape, const Shape &region, ScreenWriter *sw) const = 0;
};

#endif //CLIPPING_ALGORITHM_H
