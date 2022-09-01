#include <string>
#include <vector>
#include <ctype.h>
#include "./Token.h"

using namespace std;

string KEYWORD_LIST[] = { "procedure", "if", "else", "then", "while", "print", "read" };
char SYMBOL_LIST[] = { '(', ')', '{', '}', ';' };
char WHITESPACE_LIST[] = { ' ', '\n', '\t' };
string OPERATOR_LIST[] = { "+", "-",  "=", "*", "/", "%" , ">", "<", ">=", "<=", "==", "!=", "!", "&&", "||" };
char opChar[] = { '+', '-', '=', '*', '/', '%', '>', '<', '!', '&', '|' }; 

enum TokenType {
	WHITESPACE, // act as default token type
	NAME,
	CONSTANT,
	KEYWORD, // word that decide stmt type 
	OPERATOR, // cond, rel, arithmetic symbols
	SYMBOL // parenthesis and ';'
};

class Tokenizer{
private:
	string& input = {};

	Token createToken(TokenType type, string value) {
		switch (type) {
			case CONSTANT: return new Constant(value);
			case NAME: return new Name(value);
			case KEYWORD: return new Keyword(value);
			case OPERATOR: return new Operator(value);
			case SYMBOL: return new Symbol(value);
		}
	}

public:
	Tokenizer(const string &sourceProg) {
		this->input = sourceProg;
	}

	vector<Token> tokenize() {
		vector<Token> tokens;
		TokenType currType = WHITESPACE;

		string current = ""; // store current Token value
		char currChar;
		long index = 0;

		// Iterate throught every char in the source program
		while (index < input.length()) {
			currChar = input.at(index);
			
			if (isdigit(currChar)) {  
				if (currType == WHITESPACE) {
					currType = CONSTANT;
				}
				current.append(1, currChar);
			}
			else if (isalpha(currChar)) {
				if (currType == WHITESPACE) {
					currType = NAME;
				}
				current.append(1, currChar);
			}
			else if (find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), currChar) != end(WHITESPACE_LIST)) {
				// when currType is Name, check if it is keyword
				if (currType == NAME && find(begin(KEYWORD_LIST), end(KEYWORD_LIST), currChar) != end(KEYWORD_LIST)) {
					while(index + 1 < input.length()){
						if (find(begin(WHITESPACE_LIST), end(WHITESPACE_LIST), currChar) != end(WHITESPACE_LIST)) {
							break;
						}
						index = index + 1;
					}
					char temp = input.at(index + 1);
					if (find(begin(opChar), end(opChar), temp) != end(opChar)) {
						tokens.push_back(createToken(KEYWORD, current));
					}
					else {
						tokens.push_back(createToken(NAME, current))
					}
				}
				else {
					if (currType != WHITESPACE) {
						tokens.push_back(createToken(currType, current));
					}
				}
				current = "";
				currType = WHITESPACE;
			}
			else if (find(begin(OPERATOR_LIST), end(OPERATOR_LIST), currChar) != end(OPERATOR_LIST)) {
				if (currType == WHITESPACE) {
					current = "";
					currType = OPERATOR;
				}

				if (currType == OPERATOR) {
					current.append(1, currChar);
				}
				else {
					tokens.push_back(createToken(currType, current));
				}
			}
			else if (find(begin(SYMBOL_LIST), end(SYMBOL_LIST), currChar) != end(SYMBOL_LIST){
				if (currType != WHITESPACE) {
					tokens.push_back(createToken(currType, current));
				}
				currType = SYMBOL;
				current = "";
			}
			else { 
				//TODO: invalid character throw error
			}
		}
	}
};
