#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

using namespace std;

enum class TokenType;

class Tokenizer {
private:
	string input;
	Token* createToken(TokenType type, string value);
public:
	Tokenizer(string sourceProg);
	vector<Token*> tokenize();
};
