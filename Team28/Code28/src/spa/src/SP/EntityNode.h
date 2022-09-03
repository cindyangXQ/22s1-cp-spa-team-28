#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include "../commons/Variable.h"
#include "../commons/Constant.h"


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
	bool isConstant();
};

class VariableNode : public Token, public EntityNode {
public:
	VariableNode(string s);
	VariableNode();
	bool isName();
	bool isKeyword();
	bool isConstant();
};

class StatementNode : public EntityNode {
	int line;
public:
	StatementNode();
	virtual bool isRead() {};
	virtual bool isPrint() {};
	virtual bool isCall() {};
	virtual bool isAssign() {};
	virtual string getVariable() {};
	virtual vector<Variable*> getVariables() {};
	virtual vector<Constant*> getConstants() {};
	int getLineNumber();
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
	ProgramNode();
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
	string getVariable();
	vector<Variable*> getVariables();
	vector<Constant*> getConstants();
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(VariableNode variable);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	string getVariable();
	vector<Variable*> getVariables();
	vector<Constant*> getConstants();
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(VariableNode variable);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	string getVariable();
	vector<Variable*> getVariables();
	vector<Constant*> getConstants();
};


class ExpressionNode : public EntityNode {
	Token* token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token* token);
	ExpressionNode();
	vector<Variable*> getVariables();
	vector<Constant*> getConstants();
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
	string getVariable();
	vector<Variable*> getVariables();
	vector<Constant*> getConstants();
};
