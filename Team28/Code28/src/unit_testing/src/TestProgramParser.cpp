#include "catch.hpp"
#include "SP/Parser.h"
#include "SP/Tokenizer.h"

#include <iostream>
#include <vector>

TEST_CASE("Parser parses program to statements") {
	VariableNode var1("west"), var2("y"), var3("z"), var4("west");
	Operator op("+");
	ExpressionNode expr(&op);
	AssignStatementNode stmt1(var1, &expr, 1), stmt2(var2, &expr, 2), stmt3(var3, &expr, 3), stmt4(var4, &expr, 4);
	vector<StatementNode*> stmt_list = { &stmt1, &stmt2, &stmt3, &stmt4 };
	ProcedureNode procedure1("Bedok", stmt_list);
	vector<ProcedureNode*> proc_list = { &procedure1 };
	ProgramNode expected = ProgramNode(proc_list);

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	REQUIRE(program->equals(&expected));
}
