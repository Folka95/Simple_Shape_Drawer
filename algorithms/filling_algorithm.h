#pragma once
#ifndef FILLINGALGORITHM_H
#define FILLINGALGORITHM_H
#include "../core/shape.h"

class FillingAlgorithm {
private:
    FillingAlgorithm() { }
public:
    void fill(const Shape &shape, COLORREF color);
};

#endif
