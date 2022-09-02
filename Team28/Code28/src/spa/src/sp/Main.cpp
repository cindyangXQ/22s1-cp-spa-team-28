#include "Tokenizer.h"
#include "SimpleParser.h"
#include "Entity.h"
#include <iostream>

using namespace std;

int main() {
	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest\n= 9 + east + west;\n}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token> token_list = tokenizer.tokenize();
	ProgramParser parser = ProgramParser();
	Program program = parser.parse(token_list).entity;

	for (int i = 0; i < token_list.size(); i++) {
		Token temp = token_list[i];
		std::cout << temp.value << endl;
	}

	return 0;
}
