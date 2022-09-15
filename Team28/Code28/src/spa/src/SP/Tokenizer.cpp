#include "Tokenizer.h"
#include "Token.h"
#include "EntityNode.h"
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

using namespace std;

string KEYWORD_LIST[] = { "procedure", "if", "else", "then", "while", "print", "read", "call", "then"};
char SYMBOL_LIST[] = { '(', ')', '{', '}', ';' };
char WHITESPACE_LIST[] = { ' ' , '\n', '\t' };
string OPERATOR_LIST[] = { "+", "-",  "=", "*", "/", "%" , ">", "<", ">=", "<=","!=", "!", "&&", "||" };
char opChar[] = { '+', '-', '=', '*', '/', '%', '>', '<', '!', '&', '|' };

enum class TokenType {
	WHITESPACE, // act as default token type
	NAME,
	CONSTANT,
	KEYWORD, // word that decide stmt type 
	OPERATOR, // cond, rel, arithmetic symbols
	SYMBOL // parenthesis and ';'
};


Token* Tokenizer::createToken(TokenType type, string value) {
	switch (type) {
	case TokenType::CONSTANT: return new ConstantNode(value);
	case TokenType::NAME: return new VariableNode(value);
	case TokenType::KEYWORD: return new Keyword(value);
	case TokenType::OPERATOR: return new Operator(value);
	default: return new Symbol(value);
	}
	//return Token(value);
}


Tokenizer::Tokenizer(string sourceProg) {
	this->input = sourceProg;
	this->currType = TokenType::WHITESPACE;
	this->index = 0;
}


void Tokenizer::handleDigit() {
	string constant = "";
	char currChar = input.at(index);

	while (isdigit(currChar)) {
		constant.append(1, currChar);
		
		if (++index >= input.length()) break;
		else currChar = input.at(index);
	}

	tokens.push_back(createToken(TokenType::CONSTANT, constant));
}


void Tokenizer::handleAlpha() {
	string name = "";
	char currChar = input.at(index);
	TokenType type = TokenType::NAME;

	while (isalpha(currChar) || isdigit(currChar)) {
		name.append(1, currChar);
		
		if (++index >= input.length()) break;
		else currChar = input.at(index);
	}

	if (index < input.length() && find(begin(KEYWORD_LIST), end(KEYWORD_LIST), name) != end(KEYWORD_LIST)) {
		skipWhitespace();
		if (index < input.length()) {
			currChar = input.at(index);

			if (currChar == '{') {
				if (tokens.back()->isKeyword()) type = TokenType::NAME;
				else type = TokenType::KEYWORD;
			}
			else if (find(begin(opChar), end(opChar), currChar) != end(opChar) || currChar == ';' || currChar == ')') {
				type = TokenType::NAME;
			}
			else {
				type = TokenType::KEYWORD;
			}
		}
	}

	tokens.push_back(createToken(type, name));
}


void Tokenizer::handleOperator() {
	string op = "";
	char currChar = input.at(index);
	
	while (find(begin(opChar), end(opChar), currChar) != end(opChar)) {
		op.append(1, currChar);

		if (++index >= input.length()) break;
		else currChar = input.at(index);
	}

	if (find(begin(OPERATOR_LIST), end(OPERATOR_LIST), op) == end(OPERATOR_LIST)) {
		throw "invalid operator";
	}

	tokens.push_back(createToken(TokenType::OPERATOR, op));
}


void Tokenizer::handleSymbol() {
	string sym = "";
	sym.append(1, input.at(index));
	tokens.push_back(createToken(TokenType::SYMBOL, sym));
	index++;
}

void Tokenizer::skipWhitespace() {
	char currChar = input.at(index);

	while (index < input.length() && find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), input.at(index)) != end(WHITESPACE_LIST)) index++;
}


vector<Token*> Tokenizer::tokenize() {
	string current = ""; // store current Token value
	char currChar;

	// Iterate throught every char in the source program
	while (index < input.length()) {
		currChar = input.at(index);

		if (isdigit(currChar)) {
			handleDigit();
		}
		else if (isalpha(currChar)) {
			handleAlpha();
		}
		else if (find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), currChar) != end(WHITESPACE_LIST)) {
			skipWhitespace();
		}
		else if (find(begin(opChar), end(opChar), currChar) != end(opChar)) {
			handleOperator();
		}
		else if (find(begin(SYMBOL_LIST), end(SYMBOL_LIST), currChar) != end(SYMBOL_LIST)) {
			handleSymbol();
		}
		else {
			throw "invalid character";
		}
	}

	return this->tokens;
}
