#include "EntityNode.h"
#include "SPUtils.h"
#include <vector>

EntityNode::EntityNode() {}
StatementNode::StatementNode() { this->line = 0; }

// Program
ProgramNode::ProgramNode(std::vector<ProcedureNode *> procList) {
    this->procList = procList;
}

ProgramNode::ProgramNode() {}

std::vector<ProcedureNode *> ProgramNode::getProcList() {
    return this->procList;
}

bool ProgramNode::equals(ProgramNode *other) {
    std::vector<ProcedureNode *> procedures = this->getProcList();
    std::vector<ProcedureNode *> others = other->getProcList();
    return SPUtils::compareProcList(procedures, others);
}

// Procedure
ProcedureNode::ProcedureNode(std::string procName,
                             std::vector<StatementNode *> stmtList) {
    this->procName = procName;
    this->stmtList = stmtList;
}

bool ProcedureNode::equals(ProcedureNode *other) {
    if (this->getName() != other->getName()) {
        return false;
    }
    std::vector<StatementNode *> statements = this->getStmtList();
    std::vector<StatementNode *> others = other->getStmtList();
    return SPUtils::compareStmtList(statements, others);
}

std::string ProcedureNode::getName() { return this->procName; }

std::vector<StatementNode *> ProcedureNode::getStmtList() {
    return this->stmtList;
}

int ProcedureNode::getEndline() { return stmtList.back()->getEndLine(); }

// Statement
std::vector<std::string> *StatementNode::getUsesInto(
    std::vector<Relationship<int, std::string> *> &result) {
    return new std::vector<std::string>();
}

std::vector<std::string> *StatementNode::getModsInto(
    std::vector<Relationship<int, std::string> *> &result) {
    return new std::vector<std::string>();
}

// Statement - equals
bool ReadStatementNode::equals(StatementNode *other) {
    return other->isRead() && this->getVariable() == other->getVariable();
}

bool PrintStatementNode::equals(StatementNode *other) {
    return other->isPrint() && this->getVariable() == other->getVariable();
}

bool CallStatementNode::equals(StatementNode *other) {
    return other->isCall() && this->getVariable() == other->getVariable();
}

bool AssignStatementNode::equals(StatementNode *other) {
    // Expressions not checked
    return other->isAssign() && this->getVariable() == other->getVariable();
}

bool WhileStatementNode::equals(StatementNode *other) {
    if (other->isWhile()) {
        WhileStatementNode *temp = static_cast<WhileStatementNode *>(other);
        return this->cond->equals(temp->cond) &&
               SPUtils::compareStmtList(this->getStmtList(),
                                        temp->getStmtList());
    } else {
        return false;
    }
}

bool IfStatementNode::equals(StatementNode *other) {
    if (other->isIf()) {
        IfStatementNode *temp = static_cast<IfStatementNode *>(other);
        if (this->cond->equals(temp->cond) &&
            this->ifBlock.size() == temp->ifBlock.size() &&
            this->elseBlock.size() == temp->elseBlock.size()) {
            std::vector<StatementNode *> stmtList1 = this->getStmtList();
            std::vector<StatementNode *> stmtList2 = this->getStmtList();

            return SPUtils::compareStmtList(stmtList1, stmtList2);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Read Statement
ReadStatementNode::ReadStatementNode(VariableNode *VariableNode, int line) {
    this->var = VariableNode;
    this->line = line;
}

std::string ReadStatementNode::getVariable() { return this->var->getValue(); }

void ReadStatementNode::getVariablesInto(std::vector<std::string> &result) {
    result.push_back(this->getVariable());
}

void ReadStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    result.push_back(new Statement(line, StatementType::READ));
}

std::vector<std::string> *ReadStatementNode::getModsInto(
    std::vector<Relationship<int, std::string> *> &result) {
    Relationship<int, std::string> *mdfdVar =
        new Relationship<int, std::string>(RelationshipReference::MODIFIES,
                                           this->getLineNumber(),
                                           this->getVariable());
    result.push_back(mdfdVar);

    std::vector<std::string> *mdfd = new std::vector<std::string>();
    mdfd->push_back(this->getVariable());
    return mdfd;
}

void ReadStatementNode::getModifiesPInto(
    std::vector<std::string> &result, std::vector<ProcedureNode *> &procList) {
    result.push_back(this->getVariable());
}

// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode *VariableNode, int line) {
    this->var = VariableNode;
    this->line = line;
}

std::string PrintStatementNode::getVariable() { return this->var->getValue(); }

void PrintStatementNode::getVariablesInto(std::vector<std::string> &result) {
    result.push_back(this->getVariable());
}

void PrintStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    result.push_back(new Statement(line, StatementType::PRINT));
}

std::vector<std::string> *PrintStatementNode::getUsesInto(
    std::vector<Relationship<int, std::string> *> &result) {
    std::string variable = this->getVariable();
    Relationship<int, std::string> *temp = new Relationship<int, std::string>(
        RelationshipReference::USES, this->getLineNumber(), variable);
    result.push_back(temp);

    std::vector<std::string> *used = new std::vector<std::string>();
    used->push_back(variable);
    return used;
}

void PrintStatementNode::getUsesPInto(std::vector<std::string> &result,
                                      std::vector<ProcedureNode *> &procList) {
    result.push_back(this->getVariable());
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode *VariableNode, int line) {
    this->var = VariableNode;
    this->line = line;
}

std::string CallStatementNode::getVariable() { return this->var->getValue(); }

void CallStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    result.push_back(new Statement(line, StatementType::CALL));
}

void CallStatementNode::getUsesPInto(std::vector<std::string> &result,
                                     std::vector<ProcedureNode *> &procList) {
    ProcedureNode *procedure = SPUtils::findProc(this->getVariable(), procList);
    std::vector<std::string> temp = SPUtils::usesP(procedure, procList);
    for (size_t i = 0; i < temp.size(); i++) {
        result.push_back(temp[i]);
    }
}

void CallStatementNode::getModifiesPInto(
    std::vector<std::string> &result, std::vector<ProcedureNode *> &procList) {
    ProcedureNode *procedure = SPUtils::findProc(this->getVariable(), procList);
    std::vector<std::string> temp = SPUtils::modifiesP(procedure, procList);
    for (size_t i = 0; i < temp.size(); i++) {
        result.push_back(temp.at(i));
    }
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode *VariableNode,
                                         ExpressionNode *expression, int line) {
    var = VariableNode;
    expr = expression;
    this->line = line;
}

std::string AssignStatementNode::getVariable() { return this->var->getValue(); }

std::string AssignStatementNode::getExpressionString() {
    return this->expr->toString();
}

void AssignStatementNode::getVariablesInto(std::vector<std::string> &result) {
    result.push_back(this->getVariable());
    this->expr->getVariablesInto(result);
}

void AssignStatementNode::getConstantsInto(std::vector<std::string> &result) {
    this->expr->getConstantsInto(result);
}

void AssignStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    result.push_back(new Statement(line, StatementType::ASSIGN));
}

void AssignStatementNode::getAssignmentsInto(
    std::vector<Assignment *> &result) {
    int lineNo = this->getLineNumber();
    std::string leftVar = this->getVariable();
    std::string expression = this->getExpressionString();
    result.push_back(new Assignment(lineNo, leftVar, expression));
}

std::vector<std::string> *AssignStatementNode::getUsesInto(
    std::vector<Relationship<int, std::string> *> &result) {
    int lineNo = this->getLineNumber();
    std::vector<std::string> *used = new std::vector<std::string>();

    std::vector<std::string> variables;
    this->expr->getVariablesInto(variables);
    for (size_t i = 0; i < variables.size(); i++) {
        Relationship<int, std::string> *variable =
            new Relationship<int, std::string>(RelationshipReference::USES,
                                               lineNo, variables[i]);
        result.push_back(variable);
        used->push_back(variables[i]);
    }

    return used;
}

void AssignStatementNode::getUsesPInto(std::vector<std::string> &result,
                                       std::vector<ProcedureNode *> &procList) {
    this->expr->getVariablesInto(result);
}

std::vector<std::string> *AssignStatementNode::getModsInto(
    std::vector<Relationship<int, std::string> *> &result) {
    Relationship<int, std::string> *mdfdVar =
        new Relationship<int, std::string>(RelationshipReference::MODIFIES,
                                           this->getLineNumber(),
                                           this->getVariable());
    result.push_back(mdfdVar);

    std::vector<std::string> *mdfd = new std::vector<std::string>();
    mdfd->push_back(this->getVariable());
    return mdfd;
}

void AssignStatementNode::getModifiesPInto(
    std::vector<std::string> &result, std::vector<ProcedureNode *> &procList) {
    result.push_back(this->getVariable());
}

// While Statement
WhileStatementNode::WhileStatementNode(
    const std::vector<StatementNode *> &stmtList, ExpressionNode *cond,
    int line) {
    this->stmtList = stmtList;
    this->cond = cond;
    this->line = line;
}

int WhileStatementNode::getEndLine() {
    return this->stmtList.back()->getEndLine();
}

void WhileStatementNode::getVariablesInto(std::vector<std::string> &result) {
    cond->getVariablesInto(result);
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getVariablesInto(result);
    }
}

void WhileStatementNode::getConstantsInto(std::vector<std::string> &result) {
    cond->getConstantsInto(result);
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getConstantsInto(result);
    }
}

void WhileStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    result.push_back(new Statement(line, StatementType::WHILE));
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getStatementsInto(result);
    }
}

void WhileStatementNode::getAssignmentsInto(std::vector<Assignment *> &result) {
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getAssignmentsInto(result);
    }
}

void WhileStatementNode::getFollowsInto(
    std::vector<Relationship<int, int> *> &result) {
    SPUtils::follows(this->getStmtList(), result);
}

void WhileStatementNode::getFollowsTInto(
    std::vector<Relationship<int, int> *> &result) {
    SPUtils::followsT(this->getStmtList(), result);
}

std::vector<std::string> *WhileStatementNode::getUsesInto(
    std::vector<Relationship<int, std::string> *> &result) {
    int lineNo = this->getLineNumber();
    std::vector<std::string> *descendants = new std::vector<std::string>();

    std::vector<std::string> condVars;
    this->cond->getVariablesInto(condVars);
    for (size_t i = 0; i < condVars.size(); i++) {
        Relationship<int, std::string> *condVar =
            new Relationship<int, std::string>(RelationshipReference::USES,
                                               lineNo, condVars[i]);
        result.push_back(condVar);
        descendants->push_back(condVars[i]);
    }

    std::vector<StatementNode *> stmts = this->getStmtList();
    for (size_t i = 0; i < stmts.size(); i++) {
        std::vector<std::string> *usedVars = stmts[i]->getUsesInto(result);
        for (size_t j = 0; j < usedVars->size(); j++) {
            Relationship<int, std::string> *usedVar =
                new Relationship<int, std::string>(RelationshipReference::USES,
                                                   lineNo, usedVars->at(j));
            result.push_back(usedVar);
            descendants->push_back(usedVars->at(j));
        }
    }

    return descendants;
}

void WhileStatementNode::getUsesPInto(std::vector<std::string> &result,
                                      std::vector<ProcedureNode *> &procList) {
    this->cond->getVariablesInto(result);

    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getUsesPInto(result, procList);
    }
}

std::vector<std::string> *WhileStatementNode::getModsInto(
    std::vector<Relationship<int, std::string> *> &result) {
    int lineNo = this->getLineNumber();

    std::vector<std::string> *descendants = new std::vector<std::string>();
    std::vector<StatementNode *> stmts = this->getStmtList();
    for (size_t i = 0; i < stmts.size(); i++) {
        std::vector<std::string> *mdfdVars = stmts[i]->getModsInto(result);
        for (size_t j = 0; j < mdfdVars->size(); j++) {
            Relationship<int, std::string> *mdfdVar =
                new Relationship<int, std::string>(
                    RelationshipReference::MODIFIES, lineNo, mdfdVars->at(j));
            result.push_back(mdfdVar);
            descendants->push_back(mdfdVars->at(j));
        }
    }

    return descendants;
}

void WhileStatementNode::getModifiesPInto(
    std::vector<std::string> &result, std::vector<ProcedureNode *> &procList) {
    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getModifiesPInto(result, procList);
    }
}

void WhileStatementNode::getWhileConVar(
    std::vector<Relationship<int, std::string>*>& result) {
    std::vector<std::string> condVars;
    cond->getVariablesInto(condVars);
    int lineNo = this->getLineNumber();
    for (size_t i = 0; i < condVars.size(); i++) {
        result.push_back(new Relationship<int, std::string>(
            RelationshipReference::USES, lineNo, condVars[i]));
    }

    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getWhileConVar(result);
    }
}

void WhileStatementNode::getBranchInInto(
    std::vector<Relationship<int, int> *> &result) {
    result.push_back(new Relationship<int, int>(RelationshipReference::NEXT,
                                                line, line + 1));

    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getBranchInInto(result);
    }
}

void WhileStatementNode::getBranchOutInto(
    std::vector<Relationship<int, int> *> &result, int nextLine) {
    std::vector<StatementNode *> stmtList = getStmtList();

    if (!stmtList.back()->isIf()) {
        result.push_back(
            new Relationship<int, int>(RelationshipReference::NEXT,
                                       stmtList.back()->getLineNumber(), line));
    }

    for (size_t i = 0; i < stmtList.size() - 1; i++) {
        stmtList.at(i)->getBranchOutInto(result, stmtList.at(i+1)->getLineNumber());
    }

     stmtList.back()->getBranchOutInto(result, line);
}

// If Statement
IfStatementNode::IfStatementNode(std::vector<StatementNode *> &ifBlock,
                                 std::vector<StatementNode *> &elseBlock,
                                 ExpressionNode *cond, int line) {
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
    this->cond = cond;
    this->line = line;
}

int IfStatementNode::getEndLine() { return elseBlock.back()->getEndLine(); }

std::vector<StatementNode *> IfStatementNode::getStmtList() {
    std::vector<StatementNode *> stmtList;
    for (size_t i = 0; i < ifBlock.size(); i++) {
        stmtList.push_back(ifBlock.at(i));
    }

    for (size_t i = 0; i < elseBlock.size(); i++) {
        stmtList.push_back(elseBlock.at(i));
    }

    return stmtList;
}

void IfStatementNode::getVariablesInto(std::vector<std::string> &result) {
    std::vector<StatementNode *> stmtList = this->getStmtList();

    cond->getVariablesInto(result);
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getVariablesInto(result);
    }
}

void IfStatementNode::getConstantsInto(std::vector<std::string> &result) {
    std::vector<StatementNode *> stmtList = this->getStmtList();

    cond->getConstantsInto(result);
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getConstantsInto(result);
    }
}

void IfStatementNode::getStatementsInto(std::vector<Statement *> &result) {
    std::vector<StatementNode *> stmtList = this->getStmtList();
    result.push_back(new Statement(line, StatementType::IF));
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getStatementsInto(result);
    }
}

void IfStatementNode::getAssignmentsInto(std::vector<Assignment *> &result) {
    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList.at(i)->getAssignmentsInto(result);
    }
}

void IfStatementNode::getFollowsInto(
    std::vector<Relationship<int, int> *> &result) {
    SPUtils::follows(this->ifBlock, result);
    SPUtils::follows(this->elseBlock, result);
}

void IfStatementNode::getFollowsTInto(
    std::vector<Relationship<int, int> *> &result) {
    SPUtils::followsT(this->ifBlock, result);
    SPUtils::followsT(this->elseBlock, result);
}

std::vector<std::string> *IfStatementNode::getUsesInto(
    std::vector<Relationship<int, std::string> *> &result) {
    int lineNo = this->getLineNumber();
    std::vector<std::string> *descendants = new std::vector<std::string>();

    std::vector<std::string> condVars;
    this->cond->getVariablesInto(condVars);
    for (size_t i = 0; i < condVars.size(); i++) {
        Relationship<int, std::string> *condVar =
            new Relationship<int, std::string>(RelationshipReference::USES,
                                               lineNo, condVars[i]);
        result.push_back(condVar);
        descendants->push_back(condVars[i]);
    }

    std::vector<StatementNode *> stmts = this->getStmtList();
    for (size_t i = 0; i < stmts.size(); i++) {
        std::vector<std::string> *usedVars = stmts[i]->getUsesInto(result);
        for (size_t j = 0; j < usedVars->size(); j++) {
            Relationship<int, std::string> *usedVar =
                new Relationship<int, std::string>(RelationshipReference::USES,
                                                   lineNo, usedVars->at(j));
            result.push_back(usedVar);
            descendants->push_back(usedVars->at(j));
        }
    }

    return descendants;
}

void IfStatementNode::getUsesPInto(std::vector<std::string> &result,
                                   std::vector<ProcedureNode *> &procList) {
    this->cond->getVariablesInto(result);

    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getUsesPInto(result, procList);
    }
}

std::vector<std::string> *IfStatementNode::getModsInto(
    std::vector<Relationship<int, std::string> *> &result) {
    int lineNo = this->getLineNumber();

    std::vector<std::string> *descendants = new std::vector<std::string>();
    std::vector<StatementNode *> stmts = this->getStmtList();
    for (size_t i = 0; i < stmts.size(); i++) {
        std::vector<std::string> *mdfdVars = stmts[i]->getModsInto(result);
        for (size_t j = 0; j < mdfdVars->size(); j++) {
            Relationship<int, std::string> *mdfdVar =
                new Relationship<int, std::string>(
                    RelationshipReference::MODIFIES, lineNo, mdfdVars->at(j));
            result.push_back(mdfdVar);
            descendants->push_back(mdfdVars->at(j));
        }
    }

    return descendants;
}

void IfStatementNode::getModifiesPInto(std::vector<std::string> &result,
                                       std::vector<ProcedureNode *> &procList) {
    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getModifiesPInto(result, procList);
    }
}

void IfStatementNode::getIfConVar(
    std::vector<Relationship<int, std::string> *> &result) {
    std::vector<std::string> condVars;
    cond->getVariablesInto(condVars);
    int lineNo = this->getLineNumber();
    for (size_t i = 0; i < condVars.size(); i++) {
        result.push_back(new Relationship<int, std::string>(
            RelationshipReference::USES, lineNo, condVars[i]));
    }

    std::vector<StatementNode *> stmtList = this->getStmtList();
    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getIfConVar(result);
    }
}

void IfStatementNode::getBranchInInto(
    std::vector<Relationship<int, int> *> &result) {
    int startLine = this->getLineNumber();
    int ifStart = startLine + 1;
    int elseStart = this->elseBlock[0]->getLineNumber();
    std::vector<StatementNode *> stmtList = getStmtList();

    result.push_back(new Relationship<int, int>(RelationshipReference::NEXT,
                                                startLine, ifStart));
    result.push_back(new Relationship<int, int>(RelationshipReference ::NEXT,
                                                startLine, elseStart));

    for (size_t i = 0; i < stmtList.size(); i++) {
        stmtList[i]->getBranchInInto(result);
    }
}

void IfStatementNode::getBranchOutInto(
    std::vector<Relationship<int, int> *> &result, int nextLine) {

    if (nextLine != -1) {
        if (!ifBlock.back()->isIf()) {
            result.push_back(new Relationship<int, int>(
                RelationshipReference::NEXT, ifBlock.back()->getLineNumber(),
                nextLine));
        }
        if (!elseBlock.back()->isIf()) {
            result.push_back(new Relationship<int, int>(
                RelationshipReference::NEXT, elseBlock.back()->getLineNumber(),
                nextLine));
        }
    }

    for (size_t i = 0; i < ifBlock.size() - 1; i++) {
        ifBlock.at(i)->getBranchOutInto(result, ifBlock.at(i+1)->getLineNumber());
    }
    ifBlock.back()->getBranchOutInto(result, nextLine);

    for (size_t i = 0; i < elseBlock.size() - 1; i++) {
        elseBlock.at(i)->getBranchOutInto(result,
                                        elseBlock.at(i + 1)->getLineNumber());
    }
    elseBlock.back()->getBranchOutInto(result, nextLine);
}

// Expression
ExpressionNode::ExpressionNode(Token *token) {
    this->token = token;
    this->left = nullptr;
    this->right = nullptr;
}

void ExpressionNode::getVariablesInto(std::vector<std::string> &result) {
    if (this->token->isName()) {
        result.push_back(this->token->getValue());
    } else if (this->token->isConstant()) {
    } else {
        this->left->getVariablesInto(result);
        if (this->right != nullptr) {
            this->right->getVariablesInto(result);
        }
    }
}

void ExpressionNode::getConstantsInto(std::vector<std::string> &result) {
    if (this->token->isConstant()) {
        result.push_back(this->token->getValue());
    } else if (this->token->isName()) {
    } else {
        this->left->getConstantsInto(result);
        if (this->right != nullptr) {
            this->right->getConstantsInto(result);
        }
    }
}

bool ExpressionNode::equals(ExpressionNode *other) {
    if (this->left == nullptr && this->right == nullptr) {
        return other->left == nullptr && other->right == nullptr &&
               this->token->equals(other->token);
    } else if (this->right == nullptr) {
        if (other->left == nullptr)
            return false;
        return this->token->equals(other->token) &&
               this->left->equals(other->left);
    } else {
        if (other->left == nullptr || other->right == nullptr) {
            return false;
        }
        return this->token->equals(other->token) &&
               this->left->equals(other->left) &&
               this->right->equals(other->right);
    }
}

std::string ExpressionNode::toString() {
    std::string result = "(";

    if (this->left != nullptr) {
        result += this->left->toString();
    }
    result += this->token->getValue();
    if (this->right != nullptr) {
        result += this->right->toString();
    }
    result += ")";

    return result;
}

// Constant
ConstantNode ::ConstantNode(std::string s) { this->value = s; }

// Variable
VariableNode ::VariableNode(std::string s) { this->value = s; }

VariableNode ::VariableNode() {}
