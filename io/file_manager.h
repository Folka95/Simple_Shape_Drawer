#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <bits/stdc++.h>
#include <windows.h>
#include "action.h"


class FileManager {
private:

    static std::string decimalToBinary(int decimal);
    static int binaryToDecimal(const std::string &binary);

    static void saveToFile(const std::string &content, const std::string &filename);
    static std::string loadFromFile(const std::string &filename);

    static std::string actionToString(const std::vector< Action* > &actions);
    static std::string screenToString(const std::vector< std::vector< COLORREF > > &screen);

    static std::vector< Action* > stringToAction(const std::string &binary);
    static std::vector<std::vector<COLORREF>> stringToScreen(const std::string &binary);


public:
    FileManager();
    ~FileManager();

    static void saveActions(const std::vector< Action* > &actions, const std::string &filename);
    static std::vector< Action* > loadActions(const std::string &filename);

    static void saveScreen(const std::vector< std::vector< COLORREF > > &screen, const std::string &filename);
    static std::vector<std::vector<COLORREF>> loadScreen(const std::string &filename);
};

#endif
