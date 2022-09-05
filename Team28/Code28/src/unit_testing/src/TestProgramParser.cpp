#include "catch.hpp"
#include "SP/Parser.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE("Parser parses program to statements") {
	StatementNode stmt1 = AssignStatementNode(VariableNode("west"), 1);
	StatementNode stmt2 = AssignStatementNode(VariableNode("y"), 2);
	StatementNode stmt3 = AssignStatementNode(VariableNode("z"), 3);
	StatementNode stmt4 = AssignStatementNode(VariableNode("west"), 4);
	vector<StatementNode*> stmt_list = { &stmt1, &stmt2, &stmt3, &stmt4 };
	vector<ProcedureNode*> proc_list = { &ProcedureNode("Bedok", stmt_list) };
	ProgramNode expected = ProgramNode(proc_list);

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	assert(program.equals(expected));
}
