#pragma once

#include <vector>
#include <string>
#include "Token.h"


using namespace std;

class EntityNode {
public:
	EntityNode();
};

class ConstantNode : public Token, public EntityNode {
public:
	ConstantNode(string s);
	bool isName();
	bool isKeyword();
};

class VariableNode : public Token, public EntityNode {
public:
	VariableNode(string s);
	VariableNode();
	bool isName();
	bool isKeyword();
};

class StatementNode : public EntityNode {
public:
	StatementNode();
};

class ProcedureNode : public EntityNode {
	vector<StatementNode> stmtList;

public:
	ProcedureNode(vector<StatementNode> stmtList);
};

class ProgramNode : public EntityNode {
	vector<ProcedureNode> procList;

public:
	ProgramNode(vector<ProcedureNode> procList);
};

class ReadStatementNode : public StatementNode {
	VariableNode var;

public:
	ReadStatementNode(VariableNode variable);
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(VariableNode variable);
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(VariableNode variable);
};

class ExpressionNode : public EntityNode {
	Token token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token token);
	ExpressionNode();
};

class AssignStatementNode : public StatementNode {
	VariableNode var;
	ExpressionNode expr;

public:
	AssignStatementNode(VariableNode variable, ExpressionNode expression);

};



