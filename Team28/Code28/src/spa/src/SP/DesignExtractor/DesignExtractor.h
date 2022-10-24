#include "../../PKB/Facades/PopulateFacade.h"
#include "../../commons/Constant.h"
#include "../../commons/Procedure.h"
#include "../../commons/Statement.h"
#include "../../commons/Variable.h"
#include "../ProgramParser/EntityNode.h"

#include <functional>
#include <vector>

class DesignExtractor {
protected:
    ProgramNode *program;
    PopulateFacade *storage;

public:
    DesignExtractor(ProgramNode *program, PopulateFacade *storage);
    void extractAll();
    template <class T>
    static void extractUtil(
        std::vector<T> &result, ProgramNode *program,
        std::function<void(StatementNode *stmt, std::vector<T> &result)> func) {
        std::vector<ProcedureNode *> procList = program->getProcList();
        for (size_t i = 0; i < procList.size(); i++) {
            std::vector<StatementNode *> stmtList =
                procList.at(i)->getStmtList();
            for (StatementNode *stmt : stmtList) {
                func(stmt, result);
            }
        }
    };
    template <class T>
    static void extractUtilStmtList(
        std::vector<T> &result, ProgramNode *program,
        std::function<void(std::vector<StatementNode *> stmtList,
                           std::vector<T> &result)>
            func) {
        std::vector<ProcedureNode *> procList = program->getProcList();
        for (size_t i = 0; i < procList.size(); i++) {
            std::vector<StatementNode *> stmtList =
                procList.at(i)->getStmtList();
            func(stmtList, result);
        }
    };
};

// Extract Entities
template <typename T> class EntityExtractor : public DesignExtractor {
protected:
    Designation type;

public:
    EntityExtractor(ProgramNode *program, PopulateFacade *storage,
                    Designation type);
    virtual std::vector<T *> extract() { return std::vector<T *>(); };
    virtual void populate();
};

class ProcedureExtractor : public EntityExtractor<Procedure> {

public:
    ProcedureExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Procedure *> extract();
};

class StatementExtractor : public EntityExtractor<Statement> {
    std::vector<Assignment *> assign;
    std::vector<Relationship<int, std::string_view> *> call;

public:
    StatementExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Statement *> extract();
    std::vector<Assignment *> extractAssignments() { return assign; };
    std::vector<Relationship<int, std::string_view> *> extractCalls() {
        return call;
    };
    void populate();
};

class VariableExtractor : public EntityExtractor<Variable> {
public:
    VariableExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Variable *> extract();
};

class ConstantExtractor : public EntityExtractor<Constant> {
public:
    ConstantExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Constant *> extract();
};

// Extract Abstractions
template <typename U, typename V>
class RelationExtractor : public DesignExtractor {
protected:
    Designation type;

public:
    RelationExtractor(ProgramNode *program, PopulateFacade *storage,
                      Designation type);
    virtual std::vector<Relationship<U, V> *> extract() {
        return std::vector<Relationship<U, V> *>();
    };
    virtual void populate();
};

class FollowsExtractor : public RelationExtractor<int, int> {
public:
    FollowsExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};

class FollowsExtrT : public RelationExtractor<int, int> {
public:
    FollowsExtrT(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};

class ParentExtractor : public RelationExtractor<int, int> {
public:
    ParentExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};

class ParentExtrT : public RelationExtractor<int, int> {
public:
    ParentExtrT(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};

class UsesSExtractor : public RelationExtractor<int, std::string_view> {
    std::vector<Relationship<int, std::string_view> *> ifCondVars;
    std::vector<Relationship<int, std::string_view> *> whileCondVars;

public:
    UsesSExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, std::string_view> *> extract();
    void conVar(std::vector<Relationship<int, std::string_view> *> &ifResult,
           std::vector<Relationship<int, std::string_view> *> &whileResult);
    void populate();
};

class UsesPExtractor
    : public RelationExtractor<std::string_view, std::string_view> {
public:
    UsesPExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<std::string_view, std::string_view> *> extract();
};

class ModSExtractor : public RelationExtractor<int, std::string_view> {
public:
    ModSExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, std::string_view> *> extract();
};

class ModPExtractor
    : public RelationExtractor<std::string_view, std::string_view> {
public:
    ModPExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<std::string_view, std::string_view> *> extract();
};

class CallsExtractor
    : public RelationExtractor<std::string_view, std::string_view> {
public:
    CallsExtractor(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<std::string_view, std::string_view> *> extract();
};

class CallsExtrT
    : public RelationExtractor<std::string_view, std::string_view> {
public:
    CallsExtrT(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<std::string_view, std::string_view> *> extract();
};

class BranchInExtr : public RelationExtractor<int, int> {
public:
    BranchInExtr(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};

class BranchOutExtr : public RelationExtractor<int, int> {
public:
    BranchOutExtr(ProgramNode *program, PopulateFacade *storage);
    std::vector<Relationship<int, int> *> extract();
};
