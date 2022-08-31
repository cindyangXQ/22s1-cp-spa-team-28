#pragma once

#include <string>
#include <vector>
#include <ctype.h>
#include "./Token.h"

using namespace std;

string KEYWORD_LIST[];
char SYMBOL_LIST[];
char WHITESPACE_LIST[];
string OPERATOR_LIST[]; // rel & cond op not included yet
char opChar[];

enum class TokenType;

class Tokenizer {
public:
	Tokenizer(string sourceProg);
	vector<Token> tokenize();
private:
	string input;
	Token createToken(TokenType type, string value);
};
