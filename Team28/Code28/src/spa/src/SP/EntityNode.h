#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include "../commons/Statement.h"
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
protected:
	int line;
public:
	StatementNode();
	virtual bool isRead() { return false; }
	virtual bool isPrint() { return false; }
	virtual bool isCall() { return false; }
	virtual bool isAssign() { return false; }
	virtual bool isWhile() { return false; }
	virtual bool equals(StatementNode* other) { return false; };
	virtual string getVariable() { return ""; };
  int getLineNumber() { return this -> line;  };
	virtual int getEndLine() { return this->line; }
	virtual void getVariablesInto(vector<string>& result) {};
	virtual void getConstantsInto(vector<string>& result) {};
	virtual void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::NONE)); }
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
	ReadStatementNode(VariableNode variable, int line);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::READ)); }
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(VariableNode variable, int line);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::PRINT)); }
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(VariableNode variable, int line);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::CALL)); }
};


class ExpressionNode : public EntityNode {
	Token* token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token* token);
	ExpressionNode();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
};

class AssignStatementNode : public StatementNode {
	VariableNode var;
	ExpressionNode* expr;

public:
	AssignStatementNode(VariableNode variable, ExpressionNode* expression, int line);
	bool isRead();
	bool isPrint();
	bool isCall();
	bool isAssign();
	bool equals(StatementNode* other);
	string getVariable();
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::ASSIGN)); }
};


class WhileStatementNode : public StatementNode {
	vector<StatementNode*> stmtList;
	ExpressionNode* cond;

public:
	WhileStatementNode(vector<StatementNode*> stmtList, ExpressionNode* cond, int line);
	bool isWhile() { return true; }
	int getEndLine();
	vector<StatementNode*> getStmtList() { return this->stmtList; }
	void getVariablesInto(vector<string>& result);
	void getConstantsInto(vector<string>& result);
	void getStatementsInto(vector<Statement*>& result);
};
