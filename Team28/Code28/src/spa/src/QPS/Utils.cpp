#include "Utils.h"

#include <string>
#include <vector>
#include <regex>

using namespace std;

vector<string> Utils::splitString(string str, char splitter) {
    vector<string> result;
    string current = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == splitter) {
            if (current != "") {
                result.push_back(current);
                current = "";
            }
            continue;
        }
        current += str[i];
    }
    if (current.size() != 0) {
        result.push_back(current);
    }
    return result;
}

string Utils::removeTrailingSpaces(string s) {
    return regex_replace(s, regex("^ +| +$|( ) +"), "$1");
}