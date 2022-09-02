#pragma once
#include <string>
#include <vector>
#include <regex>

class Utils {
    public:
        static std::vector<std::string> splitString(std::string str, char splitter);
        static std::string removeTrailingSpaces(std::string s);
        static std::vector<std::string> tokenize(std::string str, std::vector<char> special_chars);
        static bool in(std::vector<std::string> v, std::string item);
};
