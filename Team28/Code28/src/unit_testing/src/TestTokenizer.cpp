#include "SP/Tokenizer.h"
#include "catch.hpp"
#include "SP/ExprParser.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE() {
	string sourceProgram = "procedure Bedok {read x;}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token> token_list = tokenizer.tokenize();

	for (int i = 0; i < token_list.size(); i++) {
		Token temp = token_list[i];
		std::cout << temp.value << endl;
	}

	ProgramParser parser = ProgramParser(0, token_list);
	parser.parse();

	//will implement later
}
