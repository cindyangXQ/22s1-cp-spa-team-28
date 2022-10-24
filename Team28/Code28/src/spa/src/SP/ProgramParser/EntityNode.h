#pragma once

#include "../../commons/Assignment.h"
#include "../../commons/Constant.h"
#include "../../commons/Relationship.h"
#include "../../commons/Statement.h"
#include "../../commons/Variable.h"
#include "../Tokenizer/Token.h"
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

    void cleanup();
    bool equals(ProgramNode *other);
    std::vector<ProcedureNode *> getProcList();
};

class ProcedureNode : public EntityNode {
    std::string_view procName;
    std::vector<StatementNode *> stmtList;
    std::vector<std::string_view> allCalls;

public:
    ProcedureNode(std::string_view procName,
                  std::vector<StatementNode *> stmtList);
    void cleanup();
    bool equals(ProcedureNode *other);
    std::string_view getName();
    std::vector<StatementNode *> getStmtList();
    int getStartLine();
    int getEndline();
    std::vector<std::string_view> getAllCalls() { return allCalls; }
    void setAllCalls(std::vector<std::string_view> allCalls) {
        this->allCalls = allCalls;
    }
};

class StatementNode : public EntityNode {
protected:
    int line;

public:
    StatementNode();
    virtual void cleanup() { delete this; }
    virtual bool isRead() { return false; }
    virtual bool isPrint() { return false; }
    virtual bool isCall() { return false; }
    virtual bool isAssign() { return false; }
    virtual bool isWhile() { return false; }
    virtual bool isIf() { return false; }
    virtual bool equals(StatementNode *other) { return false; };
    virtual std::string_view getVariable() { return ""; };
    virtual std::vector<StatementNode *> getStmtList() {
        return std::vector<StatementNode *>();
    };
    int getLineNumber() { return this->line; };
    virtual int getEndLine() { return this->line; }
    virtual std::vector<std::string_view> getAllCalls() {
        return std::vector<std::string_view>{};
    }

    virtual void getVariablesInto(std::vector<std::string_view> &result){};
    virtual void getConstantsInto(std::vector<std::string_view> &result){};
    virtual void
    getStatementsInto(std::vector<Statement *> &result,
                      std::vector<Assignment *> &assign,
        std::vector<Relationship<int, std::string_view> *> &call) {
        result.push_back(new Statement(line, StatementType::NONE));
    }
    virtual void
    getFollowsInto(std::vector<Relationship<int, int> *> &result){};
    virtual void
    getFollowsTInto(std::vector<Relationship<int, int> *> &result){};
    virtual std::vector<std::string_view> *
    getUsesInto(std::vector<Relationship<int, std::string_view> *> &result);
    virtual void getUsesPInto(std::vector<std::string_view> &result,
                              std::vector<ProcedureNode *> &procList){};
    virtual std::vector<std::string_view> *
    getModsInto(std::vector<Relationship<int, std::string_view> *> &result);
    virtual void getModifiesPInto(std::vector<std::string_view> &result,
                                  std::vector<ProcedureNode *> &procList){};

    virtual void getConVar(
        std::vector<Relationship<int, std::string_view> *> &ifResult,
        std::vector<Relationship<int, std::string_view> *> &whileResult){};
    virtual void
    getBranchInInto(std::vector<Relationship<int, int> *> &result){};
    virtual void getBranchOutInto(std::vector<Relationship<int, int> *> &result,
                                  int nextLine){};
};

class ReadStatementNode : public StatementNode {
    VariableNode *var;

public:
    ReadStatementNode(VariableNode *variable, int line);

    void cleanup();

    bool isRead() { return true; };
    bool equals(StatementNode *other);
    std::string_view getVariable();

    void getVariablesInto(std::vector<std::string_view> &result);
    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    std::vector<std::string_view> *
    getModsInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getModifiesPInto(std::vector<std::string_view> &result,
                          std::vector<ProcedureNode *> &procList);
};

class PrintStatementNode : public StatementNode {
    VariableNode *var;

public:
    PrintStatementNode(VariableNode *variable, int line);

    void cleanup();

    bool isPrint() { return true; };
    bool equals(StatementNode *other);
    std::string_view getVariable();

    void getVariablesInto(std::vector<std::string_view> &result);
    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    std::vector<std::string_view> *
    getUsesInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getUsesPInto(std::vector<std::string_view> &result,
                      std::vector<ProcedureNode *> &procList);
};

class CallStatementNode : public StatementNode {
    VariableNode *var;

public:
    CallStatementNode(VariableNode *variable, int line);

    void cleanup();

    bool isCall() { return true; };
    bool equals(StatementNode *other);
    std::string_view getVariable();

    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    void getUsesPInto(std::vector<std::string_view> &result,
                      std::vector<ProcedureNode *> &procList);
    void getModifiesPInto(std::vector < std::string_view> & result,
                          std::vector<ProcedureNode *> &procList);
    std::vector<std::string_view> getAllCalls() {
        return std::vector<std::string_view>{this->getVariable()};
    }
};

class AssignStatementNode : public StatementNode {
    VariableNode *var;
    ExpressionNode *expr;

public:
    AssignStatementNode(VariableNode *variable, ExpressionNode *expression,
                        int line);
    void cleanup();

    bool isAssign() { return true; };
    bool equals(StatementNode *other);
    std::string_view getVariable();
    std::string_view getExpressionString();

    void getVariablesInto(std::vector<std::string_view> &result);
    void getConstantsInto(std::vector<std::string_view> &result);
    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    std::vector<std::string_view> *
    getUsesInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getUsesPInto(std::vector<std::string_view> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string_view> *
    getModsInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getModifiesPInto(std::vector<std::string_view> &result,
                          std::vector<ProcedureNode *> &procList);
};

class WhileStatementNode : public StatementNode {
    std::vector<StatementNode *> stmtList;
    ExpressionNode *cond;
    std::vector<std::string_view> allCalls;

public:
    WhileStatementNode(const std::vector<StatementNode *> &stmtList,
                       ExpressionNode *cond, int line);

    void cleanup();
    bool isWhile() { return true; };
    bool equals(StatementNode *other);
    int getEndLine();
    std::vector<std::string_view> getAllCalls() { return this->allCalls; }
    void setAllCalls(std::vector<std::string_view> allCalls) {
        this->allCalls = allCalls;
    }
    std::vector<StatementNode *> getStmtList() { return this->stmtList; };

    void getVariablesInto(std::vector<std::string_view> &result);
    void getConstantsInto(std::vector<std::string_view> &result);
    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    void getFollowsInto(std::vector<Relationship<int, int> *> &result);
    void getFollowsTInto(std::vector<Relationship<int, int> *> &result);
    std::vector<std::string_view> *
    getUsesInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getUsesPInto(std::vector<std::string_view> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string_view> *
    getModsInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getModifiesPInto(std::vector<std::string_view> &result,
                          std::vector<ProcedureNode *> &procList);

    void getConVar(std::vector<Relationship<int, std::string_view> *> &ifResult,
              std::vector<Relationship<int, std::string_view> *> &whileResult);
    void getBranchInInto(std::vector<Relationship<int, int> *> &result);
    void getBranchOutInto(std::vector<Relationship<int, int> *> &result,
                          int nextLine);
};

class IfStatementNode : public StatementNode {
    ExpressionNode *cond;
    std::vector<StatementNode *> ifBlock;
    std::vector<StatementNode *> elseBlock;
    std::vector<std::string_view> allCalls;

public:
    IfStatementNode(std::vector<StatementNode *> &ifBlock,
                    std::vector<StatementNode *> &elseBlock,
                    ExpressionNode *cond, int line);
    void cleanup();
    bool isIf() { return true; }
    bool equals(StatementNode *other);
    int getEndLine();
    std::vector<StatementNode *> getStmtList();
    std::vector<std::string_view> getAllCalls() { return allCalls; }
    void setAllCalls(std::vector<std::string_view> allCalls) {
        this->allCalls = allCalls;
    }

    void getVariablesInto(std::vector<std::string_view> &result);
    void getConstantsInto(std::vector<std::string_view> &result);
    void getStatementsInto(std::vector<Statement *> &result,
                           std::vector<Assignment *> &assign,
                      std::vector<Relationship<int, std::string_view> *> &call);
    void getFollowsInto(std::vector<Relationship<int, int> *> &result);
    void getFollowsTInto(std::vector<Relationship<int, int> *> &result);
    std::vector<std::string_view> *
    getUsesInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getUsesPInto(std::vector<std::string_view> &result,
                      std::vector<ProcedureNode *> &procList);
    std::vector<std::string_view> *
    getModsInto(std::vector<Relationship<int, std::string_view> *> &result);
    void getModifiesPInto(std::vector<std::string_view> &result,
                          std::vector<ProcedureNode *> &procList);

    void getConVar(std::vector<Relationship<int, std::string_view> *> &ifResult,
                   std::vector<Relationship<int, std::string_view> *> &whileResult);
    void getBranchInInto(std::vector<Relationship<int, int> *> &result);
    void getBranchOutInto(std::vector<Relationship<int, int> *> &result,
                          int nextLine);
    int getIfEndLine() { return ifBlock.back()->getEndLine(); }
};

class ExpressionNode : public EntityNode {
    Token *token;
    ExpressionNode *left;
    ExpressionNode *right;

public:
    ExpressionNode(Token *token);
    ExpressionNode();
    void cleanup();

    bool equals(ExpressionNode *other);
    Token *getToken() { return this->token; }
    std::string_view toString();

    void getVariablesInto(std::vector<std::string_view> &result);
    void getConstantsInto(std::vector<std::string_view> &result);

    void setLeft(ExpressionNode *left) { this->left = left; }
    void setRight(ExpressionNode *right) { this->right = right; }

    ExpressionNode *getLeft() { return left; }
    ExpressionNode *getRight() { return right; }
};

class VariableNode : public Token, public EntityNode {
public:
    VariableNode(std::string_view s);
    VariableNode();

    bool isName() { return true; }
    bool equals(Token *other) {
        return other->isName() && other->getValue() == this->value;
    }
};

class ConstantNode : public Token, public EntityNode {
public:
    ConstantNode(std::string_view s);
    bool isConstant() { return true; }
    bool equals(Token *other) {
        return other->isConstant() && other->getValue() == this->value;
    }
};
