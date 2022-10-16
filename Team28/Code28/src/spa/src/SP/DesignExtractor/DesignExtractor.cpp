#include "DesignExtractor.h"
#include "../ProgramParser/EntityNode.h"
#include "../SPUtils.h"
#include <algorithm>
#include <functional>
#include <map>
#include <vector>

DesignExtractor::DesignExtractor(ProgramNode *program,
                                 PopulateFacade *storage) {
    this->program = program;
    this->storage = storage;
}

template <class T>
static void DesignExtractor::extractUtil(
    std::vector<T> &result, ProgramNode *program,
    std::function<void(StatementNode *stmt, std::vector<T> &result)> func) {
    std::vector<ProcedureNode *> procList = program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::vector<StatementNode *> stmtList = procList.at(i)->getStmtList();
        for (StatementNode *stmt : stmtList) {
            func(stmt, result);
        }
    }
}

template <class T>
static void DesignExtractor::extractUtilStmtList(
    std::vector<T> &result, ProgramNode *program,
    std::function<void(std::vector<StatementNode *> stmtList,
                       std::vector<T> &result)>
        func) {
    std::vector<ProcedureNode *> procList = program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::vector<StatementNode *> stmtList = procList.at(i)->getStmtList();
        func(stmtList, result);
    }
}

std::vector<Procedure *> ProcedureExtractor::extract() {
    std::vector<Procedure *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        ProcedureNode *currProc = procList.at(i);
        result.push_back(
            new Procedure(currProc->getName(), currProc->getStartLine()));
    }

    return result;
}

std::vector<Statement *> StatementExtractor::extract() {
    std::vector<Statement *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::vector<StatementNode *> stmtList = procList.at(i)->getStmtList();
        for (size_t j = 0; j < stmtList.size(); j++) {
            StatementNode *currStmt = stmtList.at(j);
            currStmt->getStatementsInto(result, assign, call);
        }
    }

    return result;
}

std::vector<Variable *> VariableExtractor::extract() {
    std::vector<std::string> preresult;
    std::vector<Variable *> result;

    DesignExtractor::extractUtil<std::string>(
        preresult, program,
        [](StatementNode *stmt, std::vector<std::string> &res) -> void {
            stmt->getVariablesInto(res);
        });

    sort(preresult.begin(), preresult.end());
    preresult.erase(unique(preresult.begin(), preresult.end()),
                    preresult.end());

    for (size_t i = 0; i < preresult.size(); i++) {
        result.push_back(new Variable(preresult[i]));
    }

    return result;
}

std::vector<Constant *> ConstantExtractor::extract() {
    std::vector<std::string> preresult;
    std::vector<Constant *> result;

    DesignExtractor::extractUtil<std::string>(
        preresult, program,
        [](StatementNode *stmt, std::vector<std::string> &res) -> void {
            stmt->getConstantsInto(res);
        });

    sort(preresult.begin(), preresult.end());
    preresult.erase(unique(preresult.begin(), preresult.end()),
                    preresult.end());

    for (size_t i = 0; i < preresult.size(); i++) {
        result.push_back(new Constant(preresult[i]));
    }

    return result;
}

std::vector<Relationship<int, int> *> FollowsExtractor::extract() {
    std::vector<Relationship<int, int> *> result;

    DesignExtractor::extractUtilStmtList<Relationship<int, int> *>(
        result, program,
        [](std::vector<StatementNode *> stmtList,
           std::vector<Relationship<int, int> *> &res) -> void {
            SPUtils::follows(stmtList, res);
        });

    return result;
}

std::vector<Relationship<int, int> *> FollowsExtrT::extract() {
    std::vector<Relationship<int, int> *> result;

    DesignExtractor::extractUtilStmtList<Relationship<int, int> *>(
        result, program,
        [](std::vector<StatementNode *> stmtList,
           std::vector<Relationship<int, int> *> &res) -> void {
            SPUtils::followsT(stmtList, res);
        });

    return result;
}

std::vector<Relationship<int, int> *> ParentExtractor::extract() {
    std::vector<Relationship<int, int> *> result;

    DesignExtractor::extractUtil<Relationship<int, int> *>(
        result, program,
        [](StatementNode *stmt, std::vector<Relationship<int, int> *> &res)
            -> void { SPUtils::parent(stmt, res); });

    return result;
}

std::vector<Relationship<int, int> *> ParentExtrT::extract() {
    std::vector<Relationship<int, int> *> result;

    DesignExtractor::extractUtil<Relationship<int, int> *>(
        result, program,
        [](StatementNode *stmt, std::vector<Relationship<int, int> *> &res)
            -> void { SPUtils::parentT(stmt, res); });

    return result;
}

std::vector<Relationship<int, std::string> *> UsesSExtractor::extract() {
    std::vector<Relationship<int, std::string> *> result;

    DesignExtractor::extractUtil<Relationship<int, std::string> *>(
        result, program,
        [](StatementNode *stmt,
           std::vector<Relationship<int, std::string> *> &res) -> void {
            stmt->getUsesInto(res);
        });

    return result;
}

void UsesSExtractor::conVar(
    std::vector<Relationship<int, std::string> *> &ifResult,
    std::vector<Relationship<int, std::string> *> &whileResult) {
    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::vector<StatementNode *> stmtList = procList.at(i)->getStmtList();
        for (size_t j = 0; j < stmtList.size(); j++) {
            stmtList[j]->getConVar(ifResult, whileResult);
        }
    }
}

std::vector<Relationship<std::string, std::string> *>
UsesPExtractor::extract() {
    std::vector<Relationship<std::string, std::string> *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::string procName = procList[i]->getName();
        std::vector<std::string> used = SPUtils::usesP(procList[i], procList);
        for (size_t j = 0; j < used.size(); j++) {
            result.push_back(new Relationship<std::string, std::string>(
                RelationshipReference::USES, procName, used.at(j)));
        }
    }

    return result;
}

std::vector<Relationship<int, std::string> *> ModSExtractor::extract() {
    std::vector<Relationship<int, std::string> *> result;

    DesignExtractor::extractUtil<Relationship<int, std::string> *>(
        result, program,
        [](StatementNode *stmt,
           std::vector<Relationship<int, std::string> *> &res) -> void {
            stmt->getModsInto(res);
        });

    return result;
}

std::vector<Relationship<std::string, std::string> *> ModPExtractor::extract() {
    std::vector<Relationship<std::string, std::string> *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::string procName = procList[i]->getName();
        std::vector<std::string> modified =
            SPUtils::modifiesP(procList[i], procList);
        for (size_t j = 0; j < modified.size(); j++) {
            result.push_back(new Relationship<std::string, std::string>(
                RelationshipReference::MODIFIES, procName, modified.at(j)));
        }
    }

    return result;
}

std::vector<Relationship<std::string, std::string> *>
CallsExtractor::extract() {
    std::vector<Relationship<std::string, std::string> *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    for (size_t i = 0; i < procList.size(); i++) {
        std::string name = procList.at(i)->getName();
        std::vector<std::string> calls = procList.at(i)->getAllCalls();

        for (size_t j = 0; j < calls.size(); j++) {
            result.push_back(new Relationship<std::string, std::string>(
                RelationshipReference::CALLS, name, calls.at(j)));
        }
    }

    return result;
}

std::vector<Relationship<std::string, std::string> *> CallsExtrT::extract() {
    std::vector<Relationship<std::string, std::string> *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();
    std::map<std::string, std::vector<std::string>> procCallsMap;

    for (size_t i = 0; i < procList.size(); i++) {
        procCallsMap[procList[i]->getName()] = procList[i]->getAllCalls();
    }

    for (size_t i = 0; i < procList.size(); i++) {
        std::string procName = procList.at(i)->getName();
        std::vector<std::string> curr, visited;
        curr.push_back(procName);

        while (curr.size() != 0) {
            std::vector<std::string> next;
            for (size_t j = 0; j < curr.size(); j++) {
                std::vector<std::string> calls = procCallsMap[curr.at(j)];

                for (size_t k = 0; k < calls.size(); k++) {
                    if (find(begin(visited), end(visited), calls.at(k)) !=
                        end(visited)) {
                        continue;
                    }
                    visited.push_back(calls.at(k));
                    next.push_back(calls.at(k));
                    result.push_back(new Relationship<std::string, std::string>(
                        RelationshipReference::CALLS_T, procName, calls.at(k)));
                }
            }
            curr = next;
        }
    }

    return result;
}

std::vector<Relationship<int, int> *> BranchInExtr::extract() {
    std::vector<Relationship<int, int> *> result;

    DesignExtractor::extractUtil<Relationship<int, int> *>(
        result, program,
        [](StatementNode *stmt, std::vector<Relationship<int, int> *> &res)
            -> void { stmt->getBranchInInto(res); });

    return result;
}

std::vector<Relationship<int, int> *> BranchOutExtr::extract() {
    std::vector<Relationship<int, int> *> result;

    std::vector<ProcedureNode *> procList = this->program->getProcList();

    for (size_t i = 0; i < procList.size(); i++) {
        std::vector<StatementNode *> stmtList = procList[i]->getStmtList();
        for (size_t j = 0; j < stmtList.size() - 1; j++) {
            stmtList[j]->getBranchOutInto(result,
                                          stmtList[j + 1]->getLineNumber());
        }
        stmtList.back()->getBranchOutInto(result, -1);
    }

    return result;
}

void DesignExtractor::extractAll() {
    ProcedureExtractor(this->program, this->storage).populate();
    StatementExtractor(this->program, this->storage).populate();
    VariableExtractor(this->program, this->storage).populate();
    ConstantExtractor(this->program, this->storage).populate();
    FollowsExtractor(this->program, this->storage).populate();
    FollowsExtrT(this->program, this->storage).populate();
    ParentExtractor(this->program, this->storage).populate();
    ParentExtrT(this->program, this->storage).populate();
    UsesSExtractor(this->program, this->storage).populate();
    UsesPExtractor(this->program, this->storage).populate();
    ModSExtractor(this->program, this->storage).populate();
    ModPExtractor(this->program, this->storage).populate();
    CallsExtractor(this->program, this->storage).populate();
    CallsExtrT(this->program, this->storage).populate();
    BranchInExtr(this->program, this->storage).populate();
    BranchOutExtr(this->program, this->storage).populate();
}

void ProcedureExtractor::populate() {
    std::vector<Procedure *> procedures = this->extract();
    this->storage->store<Procedure>(&procedures, Designation::PROC);
}

void StatementExtractor::populate() {
    std::vector<Statement *> statements = this->extract();
    this->storage->store<Statement>(&statements, Designation::STMT);

    std::vector<Assignment *> assignments = this->extractAssignments();
    this->storage->store<Assignment>(&assignments, Designation::ASSIGN);

    std::vector<Relationship<int, std::string> *> calls = this->extractCalls();
    this->storage->store<Relationship<int, std::string>>(
        &calls, Designation::PROC_NAME);
}

void VariableExtractor::populate() {
    std::vector<Variable *> variables = this->extract();
    this->storage->store<Variable>(&variables, Designation::VAR);
}

void ConstantExtractor::populate() {
    std::vector<Constant *> constants = this->extract();
    this->storage->store<Constant>(&constants, Designation::CONST);
}

void FollowsExtractor::populate() {
    std::vector<Relationship<int, int> *> follows = this->extract();
    this->storage->store<Relationship<int, int>>(&follows,
                                                 Designation::FOLLOWS);
}

void FollowsExtrT::populate() {
    std::vector<Relationship<int, int> *> followsT = this->extract();
    this->storage->store<Relationship<int, int>>(&followsT,
                                                 Designation::FOLLOWS_T);
}

void ParentExtractor::populate() {
    std::vector<Relationship<int, int> *> parent = this->extract();
    this->storage->store<Relationship<int, int>>(&parent, Designation::PARENT);
}

void ParentExtrT::populate() {
    std::vector<Relationship<int, int> *> parentT = this->extract();
    this->storage->store<Relationship<int, int>>(&parentT,
                                                 Designation::PARENT_T);
}

void UsesSExtractor::populate() {
    std::vector<Relationship<int, std::string> *> usesS = this->extract();
    this->storage->store<Relationship<int, std::string>>(&usesS,
                                                         Designation::USE_S);

    this->conVar(ifCondVars, whileCondVars);
    this->storage->store<Relationship<int, std::string>>(&ifCondVars,
                                                         Designation::IF_C);
    this->storage->store<Relationship<int, std::string>>(&whileCondVars,
                                                         Designation::WHILE_C);
}

void UsesPExtractor::populate() {
    std::vector<Relationship<std::string, std::string> *> usesP =
        this->extract();
    this->storage->store<Relationship<std::string, std::string>>(
        &usesP, Designation::USE_P);
}

void ModSExtractor::populate() {
    std::vector<Relationship<int, std::string> *> ModifiesS = this->extract();
    this->storage->store<Relationship<int, std::string>>(&ModifiesS,
                                                         Designation::MOD_S);
}

void ModPExtractor::populate() {
    std::vector<Relationship<std::string, std::string> *> ModifiesP =
        this->extract();
    this->storage->store<Relationship<std::string, std::string>>(
        &ModifiesP, Designation::MOD_P);
}

void CallsExtractor::populate() {
    std::vector<Relationship<std::string, std::string> *> Calls =
        this->extract();
    this->storage->store<Relationship<std::string, std::string>>(
        &Calls, Designation::CALL);
}

void CallsExtrT::populate() {
    std::vector<Relationship<std::string, std::string> *> CallsT =
        this->extract();
    this->storage->store<Relationship<std::string, std::string>>(
        &CallsT, Designation::CALL_T);
}

void BranchInExtr::populate() {
    std::vector<Relationship<int, int> *> branchIn = this->extract();
    this->storage->store<Relationship<int, int>>(&branchIn, Designation::B_IN);
}

void BranchOutExtr::populate() {
    std::vector<Relationship<int, int> *> branchOut = this->extract();
    this->storage->store<Relationship<int, int>>(&branchOut,
                                                 Designation::B_OUT);
}
