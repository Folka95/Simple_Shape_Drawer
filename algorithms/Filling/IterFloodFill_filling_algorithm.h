#pragma once
#ifndef ITERFLOODFILL_FILLING_ALGORITHM_H
#define ITERFLOODFILL_FILLING_ALGORITHM_H
#include "../filling_algorithm.h"
#include <stack>


class IterFloodFill_FillingAlgorithm : public FillingAlgorithm {
public:
    IterFloodFill_FillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};



#endif
