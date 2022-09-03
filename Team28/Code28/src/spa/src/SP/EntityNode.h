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
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
};

class ProcedureNode : public EntityNode {
	string procName;
	vector<StatementNode> stmtList;

public:
	ProcedureNode(string procName, vector<StatementNode> stmtList);
	string getName();
	vector<StatementNode> getStmtList();
};

class ProgramNode : public EntityNode {
	vector<ProcedureNode> procList;

public:
	ProgramNode(vector<ProcedureNode> procList);
	vector<ProcedureNode> getProcList();
};


class ReadStatementNode : public StatementNode {
	VariableNode var;

public:
	ReadStatementNode(VariableNode variable);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(VariableNode variable);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(VariableNode variable);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
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
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
};



