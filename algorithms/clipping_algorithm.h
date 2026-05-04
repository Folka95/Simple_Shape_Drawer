#pragma once
#ifndef CLIPPING_ALGORITHM_H
#define CLIPPING_ALGORITHM_H
#include "../core/shape.h"

class ClippingAlgorithm {
private:
    ClippingAlgorithm() { }
public:
    void clip(const Shape &shape, const Shape &region, COLORREF color);
};

#endif //CLIPPING_ALGORITHM_H
