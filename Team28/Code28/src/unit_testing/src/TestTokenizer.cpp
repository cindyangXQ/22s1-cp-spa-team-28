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
	cout << program->getProcList().size() << endl;
	//ProcedureNode* procedure = program->getProcList().at(0);
	vector<Statement> statements = StatementExtractor(program).extractor();
	//cout << vars.size() << endl;
	//for (size_t i = 0; i < vars.size(); i++) {
	//	cout << vars.at(i).getName() << endl;
	//}
	for (size_t i = 0; i < statements.size(); i++) {
		cout << statements.at(i).getLineNumber();
		cout << static_cast<int>(statements.at(i).getStatementType()) << endl;
	}

	//will implement unit test later
}
