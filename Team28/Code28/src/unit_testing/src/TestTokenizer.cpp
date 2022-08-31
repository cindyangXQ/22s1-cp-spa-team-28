#include "sp/Tokenizer.h"
#include "catch.hpp"
#include <iostream>

TEST_CASE("sample program") {
	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest\n= 9 + east + west;\n}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token> token_list = tokenizer.tokenize();

	for (int i = 0; i < token_list.size(); i++) {
		Token temp = token_list[i];
		std::cout << temp.value << endl;
	}
}
