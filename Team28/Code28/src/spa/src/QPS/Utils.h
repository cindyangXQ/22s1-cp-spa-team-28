#pragma once
#include <string>
#include <vector>
#include <regex>

/*
 * Class containing helper functions for QPS.
 */
class Utils {
    public:
        static std::vector<std::string> splitString(std::string str, char splitter);
        static std::string removeTrailingSpaces(std::string s);
};
