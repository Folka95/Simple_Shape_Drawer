#pragma once
#ifndef NON_CONVEX_FILLING_ALGORITHM_H
#define NON_CONVEX_FILLING_ALGORITHM_H
#include "../../filling_algorithm.h"


class NonConvexFillingAlgorithm : public FillingAlgorithm {
private:
    void fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
public:
    NonConvexFillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};


#endif
