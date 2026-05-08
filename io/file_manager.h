#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <vector>

#include "action.h"


class file_manager {
public:
    file_manager();
    ~file_manager();
    void saveActions(const std::vector< Action > &actions, std::string filename);
    std::vector< Action > loadActions(std::string filename);
};



#endif //FILE_MANAGER_H
