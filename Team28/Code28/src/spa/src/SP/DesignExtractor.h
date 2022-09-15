#include "EntityNode.h"
#include "../commons/Procedure.h"
#include "../commons/Statement.h"
#include "../commons/Variable.h"
#include "../commons/Constant.h"
#include "../commons/Relationship/Follows.h"
#include "../PKB/PopulateFacade.h"

#include <vector>

using namespace std;

class DesignExtractor {
protected:
	ProgramNode* program;
	PopulateFacade* storage;

public:
	DesignExtractor(ProgramNode* program, PopulateFacade* storage);
	DesignExtractor();
	void extractAll();
};


// Extract Entities
class ProcedureExtractor : public DesignExtractor {
public:
	ProcedureExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Procedure*> extract();
	void populate();
};

class StatementExtractor : public DesignExtractor {
public:
	StatementExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Statement*> extract();
	void populate();
};

class VariableExtractor : public DesignExtractor {
public:
	VariableExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Variable*> extract();
	void populate();
};

class ConstantExtractor : public DesignExtractor {
public:
	ConstantExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Constant*> extract();
	void populate();
};


// Extract Abstractions
class FollowsExtractor : public DesignExtractor {
public:
	FollowsExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Relationship<int, int>*> extract();
	void populate();
};

class FollowsExtrT : public DesignExtractor {
public:
	FollowsExtrT(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Relationship<int, int>*> extract();
	void populate();
};

class ParentExtractor : public DesignExtractor {
public:
	ParentExtractor(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Relationship<int, int>*> extract();
	void populate();
};

class ParentExtrT : public DesignExtractor {
public:
	ParentExtrT(ProgramNode* program, PopulateFacade* storage) : DesignExtractor(program, storage) {};
	vector<Relationship<int, int>*> extract();
	void populate();
};
