#pragma once

#include "Token.h"
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>

enum class TokenType;

class Tokenizer {
private:
    std::string input;
    Token *createToken(TokenType type, std::string value);
    TokenType currType;
    long index;
    std::vector<Token *> tokens;

    void handleDigit();
    void handleAlpha();
    void handleOperator();
    void handleSymbol();
    void skipWhitespace();

public:
    Tokenizer(std::string sourceProg);
    std::vector<Token *> tokenize();
};
