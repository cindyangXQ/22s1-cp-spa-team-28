#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include "../commons/Statement.h"
#include "../commons/Variable.h"
#include "../commons/Constant.h"
#include "../commons/Statement.h"
#include "../commons/Relationship.h"

using namespace std;

class EntityNode {
public:
	EntityNode();
};


class ConstantNode : public Token, public EntityNode {
public:
	ConstantNode(string s);
	bool isConstant() { return true; }
	bool equals(Token* other) { return other->isConstant() && other->value == this->value; }
};

class VariableNode : public Token, public EntityNode {
public:
	VariableNode(string s);
	VariableNode();
	bool isName() { return true; }
	bool equals(Token* other) { return other->isName() && other->value == this->value; }
};

class StatementNode : public EntityNode {
protected:
	int line;
public:
	StatementNode();
	virtual bool isRead() { return false; }
	virtual bool isPrint() { return false; }
	virtual bool isCall() { return false; }
	virtual bool isAssign() { return false; }
	virtual bool isWhile() { return false; }
	virtual bool isIf() { return false; }
	virtual bool equals(StatementNode* other) { return false; };
	virtual string getVariable() { return ""; };
  	int getLineNumber() { return this -> line;  };
	virtual int getEndLine() { return this->line; }
	virtual void getVariablesInto(vector<string>& result) {};
	virtual void getConstantsInto(vector<string>& result) {};
	virtual void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::NONE)); }
	virtual void getFollowsInto(vector<Relationship<int, int>*>& result) {};
	virtual void getFollowsTInto(vector<Relationship<int, int>*>& result) {};
};

class ProcedureNode : public EntityNode {
	string procName;
	vector<StatementNode*> stmtList;

public:
	ProcedureNode(string procName, vector<StatementNode*> stmtList);
	bool equals(ProcedureNode* other);
	string getName();
	vector<StatementNode*> getStmtList();
	int getEndline();
};

class ProgramNode : public EntityNode {
	vector<ProcedureNode*> procList;

public:
	ProgramNode(vector<ProcedureNode*> procList);
	ProgramNode();
	bool equals(ProgramNode* other);
	vector<ProcedureNode*> getProcList();
};


class ReadStatementNode : public StatementNode {
	VariableNode var;

public:
	ReadStatementNode(const VariableNode& variable, int line);
	bool isRead() { return true; };
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(const VariableNode& variable, int line);
	bool isPrint() { return true; };
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(const VariableNode& variable, int line);
	bool isCall() { return true; };
	bool equals(StatementNode* other);
	void getStatementsInto(vector<Statement*>& result);
};


class ExpressionNode : public EntityNode {
	Token* token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token* token);
	ExpressionNode();
	Token* getToken() { return this->token; }
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	bool equals(ExpressionNode* other);
};

class AssignStatementNode : public StatementNode {
	VariableNode var;
	ExpressionNode* expr;

public:
	AssignStatementNode(const VariableNode& variable, ExpressionNode* expression, int line);
	bool isAssign() { return true; };
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
};


class WhileStatementNode : public StatementNode {
	vector<StatementNode*> stmtList;
	ExpressionNode* cond;

public:
	WhileStatementNode(const vector<StatementNode*>& stmtList, ExpressionNode* cond, int line);
	bool isWhile() { return true; };
	bool equals(StatementNode* other);
	int getEndLine();
	vector<StatementNode*> getStmtList() { return this->stmtList; };
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
	void getFollowsInto(vector<Relationship<int, int>*>& result);
	void getFollowsTInto(vector<Relationship<int, int>*>& result);
};


class IfStatementNode : public StatementNode {
	vector<StatementNode*> ifBlock;
	vector<StatementNode*> elseBlock;
	ExpressionNode* cond;

public:
	IfStatementNode(vector<StatementNode*>& ifBlock, vector<StatementNode*>& elseBlock, ExpressionNode* cond, int line);
	bool isIf() { return true; }
	bool equals(StatementNode* other);
	int getEndLine();
	vector<StatementNode*> getStmtList();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
	void getFollowsInto(vector<Relationship<int, int>*>& result);
	void getFollowsTInto(vector<Relationship<int, int>*>& result);
};
