#include <string>
#include <vector>
#include <ctype.h>

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
				//TODO: throw and handle exception if currType is constant

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
					if (find(begin(opChar), end(opChar), currChar) != end(opChar)) {
						tokens.push_back(new Token(KEYWORD, current));
					}
					else {
						tokens.push_back(new Token(NAME, current));
					}
				}
				else {
					tokens.push_back(new Token(currType, current));
				}
				current = "";
				currType = WHITESPACE;
			}
			else if (find(begin(OPERATOR_LIST), end(OPERATOR_LIST), currChar) != end(OPERATOR_LIST)) {
				//TODO: if currType is not WHITESPACE throw error
				if (currType == WHITESPACE) {
					current = "";
					currType = OPERATOR;
				}

				if (currType == OPERATOR) {
					current.append(currChar);
				}
				else {
					//throw error here
				}

			}
			else { 
				//TODO: invalid character throw error
			}
		}
	}


};
