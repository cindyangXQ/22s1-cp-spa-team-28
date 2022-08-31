#include "Token.h"
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

using namespace std;

string KEYWORD_LIST[] = { "procedure", "if", "else", "then", "while", "print", "read" };
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


class Tokenizer {
private:
	string input = {};

	Token createToken(TokenType type, string value) {
		cout << static_cast<int>(type) << " " << value << endl;
		switch (type) {
		case TokenType::CONSTANT: return Constant(value);
		case TokenType::NAME: return Name(value);
		case TokenType::KEYWORD: return Keyword(value);
		case TokenType::OPERATOR: return Operator(value);
		default: return Symbol(value);
		}
		//return Token(value);
	}

public:
	Tokenizer(string sourceProg) {
		this->input = sourceProg;
	}

	vector<Token> tokenize() {
		vector<Token> tokens;
		TokenType currType = TokenType::WHITESPACE;

		string current = ""; // store current Token value
		char currChar;
		long index = 0;

		// Iterate throught every char in the source program
		while (index < input.length()) {
			currChar = input.at(index);

			if (isdigit(currChar)) {
				if (currType == TokenType::WHITESPACE) {
					currType = TokenType::CONSTANT;
					current = "";
				}
				else if (currType != TokenType::CONSTANT) {
					tokens.push_back(createToken(currType, current));
					currType = TokenType::CONSTANT;
					current = "";
				}

				current.append(1, currChar);
			}
			else if (isalpha(currChar)) {
				if (currType == TokenType::WHITESPACE) {
					currType = TokenType::NAME;
					current = "";
				}
				else if (currType != TokenType::NAME) {
					tokens.push_back(createToken(currType, current));
					currType = TokenType::NAME;
					current = "";
				}
				current.append(1, currChar);
			}
			else if (currChar == ' ') {
				// when currType is Name, check if it is keyword
				if (currType == TokenType::NAME && find(begin(KEYWORD_LIST), end(KEYWORD_LIST), current) != end(KEYWORD_LIST)) {
					while (index + 1 < input.length()) {
						if (find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), currChar) != end(WHITESPACE_LIST)) {
							break;
						}
						index = index + 1;
					}
					char temp = input.at(index + 1);
					if (find(begin(opChar), end(opChar), temp) != end(opChar) && temp != ';') {
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
			else if (find(begin(opChar), end(opChar), currChar) != end(opChar)) {
				if (currType == TokenType::WHITESPACE) {
					current = "";
					currType = TokenType::OPERATOR;
				}

				if (currType == TokenType::OPERATOR) {
					current.append(1, currChar);
				}
				else {
					tokens.push_back(createToken(currType, current));
				}
			}
			else if (find(begin(SYMBOL_LIST), end(SYMBOL_LIST), currChar) != end(SYMBOL_LIST)) {
				if (currType != TokenType::WHITESPACE) {
					tokens.push_back(createToken(currType, current));
				}
				currType = TokenType::SYMBOL;
				current = "";
				current.push_back(currChar);
			}
			index = index + 1;
		}
		return tokens;
	}
};
