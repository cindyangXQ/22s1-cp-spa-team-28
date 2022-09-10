#include "SP/Tokenizer.h"
#include "catch.hpp"
#include "SP/ExprParser.h"
#include "SP/DesignExtractor.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE() {
	string sourceProgram = "procedure Bedok {\nx2 = (2 * 1 + 2) / 3;\nwhile((ppop>=3)&&((1*2)+3< y+4)){\nx = y+1;print x;}\nread t;}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token*> token_list = tokenizer.tokenize();

	for (int i = 0; i < token_list.size(); i++) {
		Token* temp = token_list[i];
	}

	ProgramParser parser = ProgramParser(0, token_list);
	ProgramNode* program = parser.parse();
	//ProcedureNode* procedure = program->getProcList().at(0);
	Storage* storage = new Storage();
	PopulateFacade facade = PopulateFacade(storage);
	//DesignExtractor(program, &facade).extractAll();
	vector<Statement*> vars = StatementExtractor(program, &facade).extract();

	
	std::cout << "__________________________" << endl;
	for (size_t i = 0; i < vars.size(); i++) {
		std::cout << vars.at(i)->getLineNumber()<<endl;
	}
}
