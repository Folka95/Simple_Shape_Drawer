#pragma once
#ifndef FLOODFILL_FILLING_ALGORITHM_H
#define FLOODFILL_FILLING_ALGORITHM_H
#include "../filling_algorithm.h"


class FloodFill_FillingAlgorithm : public FillingAlgorithm {
public:
    FloodFill_FillingAlgorithm();
    void fill(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
private:
    void fill_helper(const Shape &shape, const Shape &clippingRegion, const Point &startPoint, ScreenWriter *sw) const;
};



#endif
