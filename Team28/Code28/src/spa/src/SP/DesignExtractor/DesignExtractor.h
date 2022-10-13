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
class ProcedureExtractor : public DesignExtractor {

public:
    ProcedureExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Procedure *> extract();
    void populate();
};

class StatementExtractor : public DesignExtractor {
    std::vector<Assignment *> assign;
    std::vector<Relationship<int, std::string> *> call;

public:
    StatementExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Statement *> extract();
    std::vector<Assignment *> extractAssignments() { return assign; };
    std::vector<Relationship<int, std::string> *> extractCalls() {
        return call;
    };
    void populate();
};

class VariableExtractor : public DesignExtractor {
public:
    VariableExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Variable *> extract();
    void populate();
};

class ConstantExtractor : public DesignExtractor {
public:
    ConstantExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Constant *> extract();
    void populate();
};

// Extract Abstractions
class FollowsExtractor : public DesignExtractor {
public:
    FollowsExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};

class FollowsExtrT : public DesignExtractor {
public:
    FollowsExtrT(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};

class ParentExtractor : public DesignExtractor {
public:
    ParentExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};

class ParentExtrT : public DesignExtractor {
public:
    ParentExtrT(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};

class UsesSExtractor : public DesignExtractor {
    std::vector<Relationship<int, std::string> *> ifCondVars;
    std::vector<Relationship<int, std::string> *> whileCondVars;

public:
    UsesSExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, std::string> *> extract();
    void conVar(std::vector<Relationship<int, std::string> *> &ifResult,
                std::vector<Relationship<int, std::string> *> &whileResult);
    void populate();
};

class UsesPExtractor : public DesignExtractor {
public:
    UsesPExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<std::string, std::string> *> extract();
    void populate();
};

class ModSExtractor : public DesignExtractor {
public:
    ModSExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, std::string> *> extract();
    void populate();
};

class ModPExtractor : public DesignExtractor {
public:
    ModPExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<std::string, std::string> *> extract();
    void populate();
};

class CallsExtractor : public DesignExtractor {
public:
    CallsExtractor(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<std::string, std::string> *> extract();
    void populate();
};

class CallsExtrT : public DesignExtractor {
public:
    CallsExtrT(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<std::string, std::string> *> extract();
    void populate();
};

class BranchInExtr : public DesignExtractor {
public:
    BranchInExtr(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};

class BranchOutExtr : public DesignExtractor {
public:
    BranchOutExtr(ProgramNode *program, PopulateFacade *storage)
        : DesignExtractor(program, storage){};
    std::vector<Relationship<int, int> *> extract();
    void populate();
};