#pragma once

#include <vector>
#include <string>
#include "Token.h"


using namespace std;

class Constant : public Token, public EntityNode {
public:
	Constant(string s);
	bool isName();
	bool isKeyword();
};

class Variable : public Token, public EntityNode {
public:
	Variable(string s);
	Variable();
	bool isName();
	bool isKeyword();
};

class EntityNode {};

class ProgramNode : public EntityNode {
	vector<ProcedureNode> procList;

public:
	ProgramNode(vector<ProcedureNode> procList);
};

class ProcedureNode : public EntityNode {
	vector<StatementNode> stmtList;

public:
	ProcedureNode(vector<StatementNode> stmtList);
};

class StatementNode : public EntityNode {
public:
	StatementNode();
};

class ReadStatementNode : public StatementNode {
	Variable var;

public:
	ReadStatementNode(Variable variable);
};

class PrintStatementNode : public StatementNode {
	Variable var;

public:
	PrintStatementNode(Variable variable);
};

class CallStatementNode : public StatementNode {
	Variable var;

public:
	CallStatementNode(Variable variable);
};

class AssignStatementNode : public StatementNode {
	Variable var;
	ExpressionNode expr;

public:
	AssignStatementNode(Variable variable, ExpressionNode expression);

};

class ExpressionNode : public EntityNode {
	Token token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token token);
	ExpressionNode();
};



