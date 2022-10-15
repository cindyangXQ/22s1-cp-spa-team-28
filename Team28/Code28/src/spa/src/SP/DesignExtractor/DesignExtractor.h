#include "../../PKB/Facades/PopulateFacade.h"
#include "../../commons/Constant.h"
#include "../../commons/Procedure.h"
#include "../../commons/Statement.h"
#include "../../commons/Variable.h"
#include "../ProgramParser/EntityNode.h"

#include <vector>

class DesignExtractor {
protected:
    ProgramNode *program;
    PopulateFacade *storage;

public:
    DesignExtractor(ProgramNode *program, PopulateFacade *storage);
    void extractAll();
};

// Extract Entities
template<typename T> class EntityExtractor : public DesignExtractor {
protected:
    Populate type;

public:
    EntityExtractor(ProgramNode *program, PopulateFacade *storage,
                    Populate type);
    virtual std::vector<T *> extract() { return std::vector<T *>(); };
    virtual void populate();
};

class ProcedureExtractor : public EntityExtractor<Procedure> {

public:
    ProcedureExtractor(ProgramNode *program, PopulateFacade *storage)
        : EntityExtractor<Procedure>(program, storage, Populate::PROC){};
    std::vector<Procedure *> extract();
};

class StatementExtractor : public EntityExtractor<Statement> {
    std::vector<Assignment *> assign;
    std::vector<Relationship<int, std::string> *> call;

public:
    StatementExtractor(ProgramNode *program, PopulateFacade *storage)
        : EntityExtractor(program, storage, Populate::STMT){};
    std::vector<Statement *> extract();
    std::vector<Assignment *> extractAssignments() { return assign; };
    std::vector<Relationship<int, std::string> *> extractCalls() {
        return call;
    };
    void populate();
};

class VariableExtractor : public EntityExtractor<Variable> {
public:
    VariableExtractor(ProgramNode *program, PopulateFacade *storage)
        : EntityExtractor(program, storage, Populate::VAR){};
    std::vector<Variable *> extract();
};

class ConstantExtractor : public EntityExtractor<Constant> {
public:
    ConstantExtractor(ProgramNode *program, PopulateFacade *storage)
        : EntityExtractor(program, storage, Populate::CONST){};
    std::vector<Constant *> extract();
};

// Extract Abstractions
template <typename U, typename V> 
class RelationExtractor : public DesignExtractor {
protected:
    Populate type;

public:
    RelationExtractor(ProgramNode *program, PopulateFacade *storage,
                    Populate type);
    virtual std::vector<Relationship<U, V> *> extract() {
        return std::vector<Relationship<U, V> *>();
    };
    virtual void populate();
};

class FollowsExtractor : public RelationExtractor<int, int> {
public:
    FollowsExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::FOLLOWS){};
    std::vector<Relationship<int, int> *> extract();
};

class FollowsExtrT : public RelationExtractor<int, int> {
public:
    FollowsExtrT(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::FOLLOWS_T){};
    std::vector<Relationship<int, int> *> extract();
};

class ParentExtractor : public RelationExtractor<int, int> {
public:
    ParentExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::PARENT){};
    std::vector<Relationship<int, int> *> extract();
};

class ParentExtrT : public RelationExtractor<int, int> {
public:
    ParentExtrT(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::PARENT_T){};
    std::vector<Relationship<int, int> *> extract();
};

class UsesSExtractor : public RelationExtractor<int, std::string> {
    std::vector<Relationship<int, std::string> *> ifCondVars;
    std::vector<Relationship<int, std::string> *> whileCondVars;

public:
    UsesSExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::USE_S){};
    std::vector<Relationship<int, std::string> *> extract();
    void conVar(std::vector<Relationship<int, std::string> *> &ifResult,
                std::vector<Relationship<int, std::string> *> &whileResult);
    void populate();
};

class UsesPExtractor : public RelationExtractor<std::string, std::string> {
public:
    UsesPExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::USE_P){};
    std::vector<Relationship<std::string, std::string> *> extract();
};

class ModSExtractor : public RelationExtractor<int, std::string> {
public:
    ModSExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::MOD_S){};
    std::vector<Relationship<int, std::string> *> extract();
};

class ModPExtractor : public RelationExtractor<std::string, std::string> {
public:
    ModPExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::MOD_P){};
    std::vector<Relationship<std::string, std::string> *> extract();
};

class CallsExtractor : public RelationExtractor<std::string, std::string> {
public:
    CallsExtractor(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::CALL){};
    std::vector<Relationship<std::string, std::string> *> extract();
};

class CallsExtrT : public RelationExtractor<std::string, std::string> {
public:
    CallsExtrT(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::CALL_T){};
    std::vector<Relationship<std::string, std::string> *> extract();
};

class BranchInExtr : public RelationExtractor<int, int> {
public:
    BranchInExtr(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::B_IN){};
    std::vector<Relationship<int, int> *> extract();
};

class BranchOutExtr : public RelationExtractor<int, int> {
public:
    BranchOutExtr(ProgramNode *program, PopulateFacade *storage)
        : RelationExtractor(program, storage, Populate::B_OUT){};
    std::vector<Relationship<int, int> *> extract();
};
