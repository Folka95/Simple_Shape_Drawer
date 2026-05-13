#pragma once
#ifndef CONVEX_FILLING_ALGORITHM_H
#define CONVEX_FILLING_ALGORITHM_H
#include "../../filling_algorithm.h"
#include <climits>
#include <vector>
#include <cmath>
#include <algorithm>


class ConvexFillingAlgorithm : public FillingAlgorithm {
private:
    void fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
public:
    ConvexFillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};


#endif
