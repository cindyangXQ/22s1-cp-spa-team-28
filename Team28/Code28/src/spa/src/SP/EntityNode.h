#pragma once

#include "../commons/Assignment.h"
#include "../commons/Constant.h"
#include "../commons/Relationship.h"
#include "../commons/Statement.h"
#include "../commons/Variable.h"
#include "Token.h"
#include <string>
#include <vector>

class EntityNode {
public:
    EntityNode();
};


class ProgramNode;
class ProcedureNode;
class StatementNode;
class ExpressionNode;
class VariableNode;
class ConstantNode;

class ProgramNode : public EntityNode {
    std::vector<ProcedureNode *> procList;

public:
    ProgramNode(std::vector<ProcedureNode *> procList);
    ProgramNode();
    bool equals(ProgramNode *other);
    std::vector<ProcedureNode *> getProcList();
};

class ProcedureNode : public EntityNode {
    std::string procName;
    std::vector<StatementNode *> stmtList;
    std::vector<std::string> allCalls;

public:
    ProcedureNode(std::string procName, std::vector<StatementNode *> stmtList);
    bool equals(ProcedureNode *other);
    std::string getName();
    std::vector<StatementNode *> getStmtList();
    int getEndline();
    std::vector<std::string> getAllCalls() { return allCalls; }
    void setAllCalls(std::vector<std::string> allCalls) {
        this->allCalls = allCalls;
    }
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
    virtual bool equals(StatementNode *other) { return false; };
    virtual std::string getVariable() { return ""; };
    virtual std::vector<StatementNode *> getStmtList() {
        return std::vector<StatementNode *>();
    };
    int getLineNumber() { return this->line; };
    virtual int getEndLine() { return this->line; }

    virtual void getVariablesInto(std::vector<std::string> &result){};
    virtual void getConstantsInto(std::vector<std::string> &result){};
    virtual void getStatementsInto(std::vector<Statement *> &result) {
        result.push_back(new Statement(line, StatementType::NONE));
    }
    virtual void getAssignmentsInto(std::vector<Assignment *> &result){};
    virtual void
    getFollowsInto(std::vector<Relationship<int, int> *> &result){};
    virtual void
    getFollowsTInto(std::vector<Relationship<int, int> *> &result){};
    virtual std::vector<std::string> *
    getUsesInto(std::vector<Relationship<int, std::string> *> &result);
    virtual void getUsesPInto(std::vector<std::string> &result,
                              std::vector<ProcedureNode *> &procList){};
    virtual std::vector<std::string> *
    getModsInto(std::vector<Relationship<int, std::string> *> &result);
    virtual void getModifiesPInto(std::vector<std::string> &result,
                              std::vector<ProcedureNode *> &procList){};
};

class ReadStatementNode : public StatementNode {
    VariableNode *var;

public:
    ReadStatementNode(VariableNode *variable, int line);
    bool isRead() { return true; };
    bool equals(StatementNode *other);
    std::string getVariable();

    void getVariablesInto(std::vector<std::string> &result);
    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result) {};
    std::vector<std::string> *
    getModsInto(std::vector<Relationship<int, std::string> *> &result);
    void getModifiesPInto(std::vector<std::string> &result,
                          std::vector<ProcedureNode *> &procList);
};

class PrintStatementNode : public StatementNode {
    VariableNode *var;

public:
    PrintStatementNode(VariableNode *variable, int line);
    bool isPrint() { return true; };
    bool equals(StatementNode *other);
    std::string getVariable();

    void getVariablesInto(std::vector<std::string> &result);
    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result) {};
    std::vector<std::string> *
    getUsesInto(std::vector<Relationship<int, std::string> *> &result);
    void getUsesPInto(std::vector<std::string> &result,
                      std::vector<ProcedureNode *> &procList);
};

class CallStatementNode : public StatementNode {
    VariableNode *var;

public:
    CallStatementNode(VariableNode *variable, int line);
    bool isCall() { return true; };
    bool equals(StatementNode *other);
    std::string getVariable();

    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result){};
    void getUsesPInto(std::vector<std::string> &result,
                      std::vector<ProcedureNode *> &procList);
    void getModifiesPInto(std::vector<std::string> &result,
                          std::vector<ProcedureNode *> &procList);
};

class AssignStatementNode : public StatementNode {
    VariableNode *var;
    ExpressionNode *expr;

public:
    AssignStatementNode(VariableNode *variable, ExpressionNode *expression,
                        int line);
    bool isAssign() { return true; };
    bool equals(StatementNode *other);
    std::string getVariable();
    std::string getExpressionString();

    void getVariablesInto(std::vector<std::string> &result);
    void getConstantsInto(std::vector<std::string> &result);
    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result);
    std::vector<std::string> *
    getUsesInto(std::vector<Relationship<int, std::string> *> &result);
    void getUsesPInto(std::vector<std::string> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string> *
    getModsInto(std::vector<Relationship<int, std::string> *> &result);
    void getModifiesPInto(std::vector<std::string> &result,
                          std::vector<ProcedureNode *> &procList);
};

class WhileStatementNode : public StatementNode {
    std::vector<StatementNode *> stmtList;
    ExpressionNode *cond;

public:
    WhileStatementNode(const std::vector<StatementNode *> &stmtList,
                       ExpressionNode *cond, int line);
    bool isWhile() { return true; };
    bool equals(StatementNode *other);
    int getEndLine();
    std::vector<StatementNode *> getStmtList() { return this->stmtList; };

    void getVariablesInto(std::vector<std::string> &result);
    void getConstantsInto(std::vector<std::string> &result);
    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result);
    void getFollowsInto(std::vector<Relationship<int, int> *> &result);
    void getFollowsTInto(std::vector<Relationship<int, int> *> &result);
    std::vector<std::string> *
    getUsesInto(std::vector<Relationship<int, std::string> *> &result);
    void getUsesPInto(std::vector<std::string> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string> *
    getModsInto(std::vector<Relationship<int, std::string> *> &result);
    void getModifiesPInto(std::vector<std::string> &result,
                          std::vector<ProcedureNode *> &procList);
};

class IfStatementNode : public StatementNode {
    ExpressionNode *cond;
    std::vector<StatementNode *> ifBlock;
    std::vector<StatementNode *> elseBlock;

public:
    IfStatementNode(std::vector<StatementNode *> &ifBlock,
                    std::vector<StatementNode *> &elseBlock,
                    ExpressionNode *cond, int line);
    bool isIf() { return true; }
    bool equals(StatementNode *other);
    int getEndLine();
    std::vector<StatementNode *> getStmtList();

    void getVariablesInto(std::vector<std::string> &result);
    void getConstantsInto(std::vector<std::string> &result);
    void getStatementsInto(std::vector<Statement *> &result);
    void getAssignmentsInto(std::vector<Assignment *> &result);
    void getFollowsInto(std::vector<Relationship<int, int> *> &result);
    void getFollowsTInto(std::vector<Relationship<int, int> *> &result);
    std::vector<std::string> *
    getUsesInto(std::vector<Relationship<int, std::string> *> &result);
    void getUsesPInto(std::vector<std::string> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string> *
    getModsInto(std::vector<Relationship<int, std::string> *> &result);
    void getModifiesPInto(std::vector<std::string> &result,
                          std::vector<ProcedureNode *> &procList);
};

class ExpressionNode : public EntityNode {
    Token *token;

public:
    ExpressionNode *left;
    ExpressionNode *right;
    ExpressionNode(Token *token);
    ExpressionNode();
    bool equals(ExpressionNode *other);
    Token *getToken() { return this->token; }
    std::string toString();

    void getVariablesInto(std::vector<std::string> &result);
    void getConstantsInto(std::vector<std::string> &result);
};

class VariableNode : public Token, public EntityNode {
public:
    VariableNode(std::string s);
    VariableNode();
    bool isName() { return true; }
    bool equals(Token *other) {
        return other->isName() && other->value == this->value;
    }
};

class ConstantNode : public Token, public EntityNode {
public:
    ConstantNode(std::string s);
    bool isConstant() { return true; }
    bool equals(Token *other) {
        return other->isConstant() && other->value == this->value;
    }
};
