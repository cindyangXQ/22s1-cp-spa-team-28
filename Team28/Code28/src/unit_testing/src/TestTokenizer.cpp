#include "SP/Tokenizer.h"
#include "catch.hpp"
#include "SP/ExprParser.h"
#include "SP/DesignExtractor.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE() {
	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest= 9 + east + west;\n}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token*> token_list = tokenizer.tokenize();

	for (int i = 0; i < token_list.size(); i++) {
		Token* temp = token_list[i];
		std::cout << temp->value << endl;
	}

	ProgramParser parser = ProgramParser(0, token_list);
	ProgramNode* program = parser.parse();
	std::cout << program->getProcList().size() << endl;
	//ProcedureNode* procedure = program->getProcList().at(0);
	Storage* storage = new Storage();
	PopulateFacade facade = PopulateFacade(storage);
	vector<Variable*> vars = VariableExtractor(program).extract(facade);

	/*
	std::cout << "__________________________" << endl;
	for (size_t i = 0; i < vars.size(); i++) {
		std::cout << vars.at(i)->getName();
	}*/
}
