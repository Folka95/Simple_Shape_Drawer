#pragma once
#ifndef CONVEX_FILLING_ALGORITHM_H
#define CONVEX_FILLING_ALGORITHM_H
#include "../../filling_algorithm.h"


class convex_FillingAlgorithm : public FillingAlgorithm {
private:
    void fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
public:
    convex_FillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};


#endif
