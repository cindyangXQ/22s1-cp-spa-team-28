#pragma once

#include <string>
#include <vector>
#include <regex>

using namespace std;

class Utils {
    public:
        static vector<string> splitString(string str, char splitter);
        static string removeTrailingSpaces(string s);
};
