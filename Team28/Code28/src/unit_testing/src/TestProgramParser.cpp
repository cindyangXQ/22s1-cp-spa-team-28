#include "catch.hpp"
#include "SP/Parser.h"
#include "SP/Tokenizer.h"

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

TEST_CASE("keyword as assignment LHS") {
	VariableNode var1("west"), var2("a"), var3("b"), var4("read"), var5("print"), var6("call");
	Operator op("+");
	ExpressionNode expr(&op);
	AssignStatementNode stmt1(var1, &expr, 1);
	ReadStatementNode stmt2(var2, 2);
	PrintStatementNode stmt3(var3, 3);
	AssignStatementNode stmt4(var4, &expr, 4);
	AssignStatementNode stmt5(var5, &expr, 5);
	AssignStatementNode stmt6(var6, &expr, 6);
	vector<StatementNode*> stmt_list = { &stmt1, &stmt2, &stmt3, &stmt4, &stmt5, &stmt6 };
	ProcedureNode procedure1("Bedok", stmt_list);
	vector<ProcedureNode*> proc_list = { &procedure1 };
	ProgramNode expected = ProgramNode(proc_list);

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\nread a;\nprint b;\nread = c;\nprint = d;\ncall = e;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	REQUIRE(program->equals(&expected));
}

TEST_CASE("read read; print print") {
	VariableNode var1("read"), var2("print"), var3("call");
	Operator op("+");
	ExpressionNode expr(&op);
	ReadStatementNode stmt1(var1, 1);
	PrintStatementNode stmt2(var2, 2);
	ReadStatementNode stmt3(var3, 3);
	PrintStatementNode stmt4(var3, 4);
	vector<StatementNode*> stmt_list = { &stmt1, &stmt2, &stmt3, &stmt4 };
	ProcedureNode procedure1("Bedok", stmt_list);
	vector<ProcedureNode*> proc_list = { &procedure1 };
	ProgramNode expected = ProgramNode(proc_list);

	string sourceProgram = "procedure Bedok {\nread read;\nprint print;\nread call;\nprint call;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	REQUIRE(program->equals(&expected));
}
