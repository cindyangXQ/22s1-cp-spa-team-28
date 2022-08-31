#include "Utils.h"

std::vector<std::string> Utils::splitString(std::string str, char splitter) {
    std::vector<std::string> result;
    std::string current = "";
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

std::string Utils::removeTrailingSpaces(std::string s) {
    return regex_replace(s, std::regex("^ +| +$|( ) +"), "$1");
}

std::vector<std::string> Utils::tokenize(std::string str, std::vector<char> special_char) {
    std::vector<std::string> result;
    std::string current = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            if (current != "") {
                result.push_back(current);
                current = "";
            }
            continue;
        }
        if (find(special_char.begin(), special_char.end(), str[i]) != special_char.end()) {
            if (current != "") {
                result.push_back(current);
                current = "";
            }
            result.push_back(std::string(1, str[i]));
            continue;
        }
        current += str[i];
    }
    if (current.size() != 0) {
        result.push_back(current);
    }
    return result;
}