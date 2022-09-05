#include "Tokenizer.h"
#include "Token.h"
#include "EntityNode.h"
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

using namespace std;

string KEYWORD_LIST[] = { "procedure", "read", "print", "call", "if", "then", "else", "while" };
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
	cout << static_cast<int>(type) << " " << value << endl;
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
}

vector<Token*> Tokenizer::tokenize() {
	vector<Token*> tokens;
	TokenType currType = TokenType::WHITESPACE;

	string current = ""; // store current Token value
	char currChar;
	long index = 0;

	// Iterate throught every char in the source program
	while (index < input.length()) {
		currChar = input.at(index);

		// if current char is a digit
		if (isdigit(currChar)) {
			/*
			if previous token is some whitespace, create a constant token;
			if previous token is an operator, bracket, or semicolon, 
				push previous token and create a constant token;
			if previous token is a constant or name, add to the previous token. */
			if (currType == TokenType::WHITESPACE) {
				currType = TokenType::CONSTANT;
				current = "";
			}
			else if (currType != TokenType::CONSTANT && currType != TokenType::NAME) {
				tokens.push_back(createToken(currType, current));
				currType = TokenType::CONSTANT;
				current = "";
			}
			current.append(1, currChar);
		}
		// if current char is an alphabet
		else if (isalpha(currChar)) {
			/* 
			if previous token is some whitespace, create a name token;
			if previous token is a constant, syntax is wrong;
			if previous token is an operator, bracket, or semicolon, 
				push previous token and create a name token;
			if previous token is a name, add on to the previous token. */
			if (currType == TokenType::WHITESPACE) {
				currType = TokenType::NAME;
				current = "";
			}
			if (currType == TokenType::CONSTANT) {
				throw "constant wrong syntax";
			}
			else if (currType != TokenType::NAME) {
				tokens.push_back(createToken(currType, current));
				currType = TokenType::NAME;
				current = "";
			}
			current.append(1, currChar);
		}
		// if current char is a space
		else if (currChar == ' ') {
			/* 
			if previous token is a name, and keyword??
			if previous token is a constant, operator, bracket, or semicolon, 
				push previous token and create a whitespace token;
			if previous token is some whitespace, create a whitespace token. */
			if (currType == TokenType::NAME && find(begin(KEYWORD_LIST), end(KEYWORD_LIST), current) != end(KEYWORD_LIST)) {
				while (index + 1 < input.length()) { // why + 1? currChar is a whitespace, this loop does nothing?
					if (find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), currChar) != end(WHITESPACE_LIST)) {
						break;
					}
					index = index + 1;
				}
				char temp = input.at(index + 1);
				if (find(begin(opChar), end(opChar), temp) != end(opChar) && temp != ';') { // a char cannot be a opchar and a semicolon
					tokens.push_back(createToken(TokenType::NAME, current));
				}
				else {
					tokens.push_back(createToken(TokenType::KEYWORD, current));
				}
			}
			else {
				if (currType != TokenType::WHITESPACE) {
					tokens.push_back(createToken(currType, current));
				}
			}
			current = "";
			currType = TokenType::WHITESPACE;
		}
		// if current char is an operator
		else if (find(begin(opChar), end(opChar), currChar) != end(opChar)) {
			/* 
			if previous token is some whitespace, create an operator token; 
			if previous token is an operator, add on to the previous token;
			if previous token is a name, constant, bracket, or semicolon, 
				push the previous token and create an operator token. */
			if (currType == TokenType::WHITESPACE) {
				current = "";
				currType = TokenType::OPERATOR;
			}

			if (currType == TokenType::OPERATOR) {
				current.append(1, currChar);
			}
			else {
				tokens.push_back(createToken(currType, current));
				current = "";
				current.append(1, currChar);
				currType = TokenType::OPERATOR;
			}
		}
		// if current token is a bracket or semicolon
		else if (find(begin(SYMBOL_LIST), end(SYMBOL_LIST), currChar) != end(SYMBOL_LIST)) {
			/*
			if previous token is some whitespace, create a symbol token
			if previous token is a name, constant, operator, bracket, or semicolon, 
				push the previous token and create a symbol token. */
			if (currType != TokenType::WHITESPACE) {
				tokens.push_back(createToken(currType, current));
			}
			currType = TokenType::SYMBOL;
			current = "";
			current.push_back(currChar);
		}
		index = index + 1;
	}

	if(currType != TokenType::WHITESPACE) {
		tokens.push_back(createToken(currType, current));
	}
	return tokens;
}
