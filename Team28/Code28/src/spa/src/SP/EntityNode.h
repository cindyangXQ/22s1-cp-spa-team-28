#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include "../commons/Statement.h"
#include "../commons/Variable.h"
#include "../commons/Constant.h"
#include "../commons/Statement.h"
#include "../commons/Relationship.h"

class EntityNode {
public:
	EntityNode();
};


class ConstantNode : public Token, public EntityNode {
public:
	ConstantNode(std::string s);
	bool isConstant() { return true; }
	bool equals(Token* other) { return other->isConstant() && other->value == this->value; }
};

class VariableNode : public Token, public EntityNode {
public:
	VariableNode(std::string s);
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
	virtual std::string getVariable() { return ""; };
  	int getLineNumber() { return this -> line;  };
	virtual int getEndLine() { return this->line; }
	virtual void getVariablesInto(std::vector<std::string>& result) {};
	virtual void getConstantsInto(std::vector<std::string>& result) {};
	virtual void getStatementsInto(std::vector<Statement*>& result) { result.push_back(new Statement(line, StatementType::NONE)); }
	virtual void getFollowsInto(std::vector<Relationship<int, int>*>& result) {};
	virtual void getFollowsTInto(std::vector<Relationship<int, int>*>& result) {};
};

class ProcedureNode : public EntityNode {
	std::string procName;
	std::vector<StatementNode*> stmtList;

public:
	ProcedureNode(std::string procName, std::vector<StatementNode*> stmtList);
	bool equals(ProcedureNode* other);
	std::string getName();
	std::vector<StatementNode*> getStmtList();
	int getEndline();
};

class ProgramNode : public EntityNode {
	std::vector<ProcedureNode*> procList;

public:
	ProgramNode(std::vector<ProcedureNode*> procList);
	ProgramNode();
	bool equals(ProgramNode* other);
	std::vector<ProcedureNode*> getProcList();
};


class ReadStatementNode : public StatementNode {
	VariableNode var;

public:
	ReadStatementNode(const VariableNode& variable, int line);
	bool isRead() { return true; };
	bool equals(StatementNode* other);
	std::string getVariable();
	void getVariablesInto(std::vector<std::string>& result);
	void getStatementsInto(std::vector<Statement*>& result);
};

class PrintStatementNode : public StatementNode {
	VariableNode var;

public:
	PrintStatementNode(const VariableNode& variable, int line);
	bool isPrint() { return true; };
	bool equals(StatementNode* other);
	std::string getVariable();
	void getVariablesInto(std::vector<std::string>& result);
	void getStatementsInto(std::vector<Statement*>& result);
};

class CallStatementNode : public StatementNode {
	VariableNode var;

public:
	CallStatementNode(const VariableNode& variable, int line);
	bool isCall() { return true; };
	bool equals(StatementNode* other);
	void getStatementsInto(std::vector<Statement*>& result);
};


class ExpressionNode : public EntityNode {
	Token* token;
public:
	ExpressionNode* left;
	ExpressionNode* right;
	ExpressionNode(Token* token);
	ExpressionNode();
	Token* getToken() { return this->token; }
	void getVariablesInto(std::vector<std::string>& result);
	void getConstantsInto(std::vector<std::string>& result);
	bool equals(ExpressionNode* other);
};

class AssignStatementNode : public StatementNode {
	VariableNode var;
	ExpressionNode* expr;

public:
	AssignStatementNode(const VariableNode& variable, ExpressionNode* expression, int line);
	bool isAssign() { return true; };
	bool equals(StatementNode* other);
	std::string getVariable();
	void getVariablesInto(std::vector<std::string>& result);
	void getConstantsInto(std::vector<std::string>& result);
	void getStatementsInto(std::vector<Statement*>& result);
};


class WhileStatementNode : public StatementNode {
	std::vector<StatementNode*> stmtList;
	ExpressionNode* cond;

public:
	WhileStatementNode(const std::vector<StatementNode*>& stmtList, ExpressionNode* cond, int line);
	bool isWhile() { return true; };
	bool equals(StatementNode* other);
	int getEndLine();
	std::vector<StatementNode*> getStmtList() { return this->stmtList; };
	void getVariablesInto(std::vector<std::string>& result);
	void getConstantsInto(std::vector<std::string>& result);
	void getStatementsInto(std::vector<Statement*>& result);
	void getFollowsInto(std::vector<Relationship<int, int>*>& result);
	void getFollowsTInto(std::vector<Relationship<int, int>*>& result);
};


class IfStatementNode : public StatementNode {
	std::vector<StatementNode*> ifBlock;
	std::vector<StatementNode*> elseBlock;
	ExpressionNode* cond;

public:
	IfStatementNode(std::vector<StatementNode*>& ifBlock, std::vector<StatementNode*>& elseBlock, ExpressionNode* cond, int line);
	bool isIf() { return true; }
	bool equals(StatementNode* other);
	int getEndLine();
	std::vector<StatementNode*> getStmtList();
	void getVariablesInto(std::vector<std::string>& result);
	void getConstantsInto(std::vector<std::string>& result);
	void getStatementsInto(std::vector<Statement*>& result);
	void getFollowsInto(std::vector<Relationship<int, int>*>& result);
	void getFollowsTInto(std::vector<Relationship<int, int>*>& result);
};
