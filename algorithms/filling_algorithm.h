#pragma once
#ifndef FILLINGALGORITHM_H
#define FILLINGALGORITHM_H
#include "../core/shape.h"

class FillingAlgorithm {
private:
    FillingAlgorithm() { }
public:
    virtual void fill(const Shape &shape) = 0;
};

#endif
