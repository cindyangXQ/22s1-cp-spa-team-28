#include "EntityNode.h"
#include "../commons/Constant.h"
#include "../commons/Procedure.h"
#include "../commons/Variable.h"
#include "../commons/Statement.h"
#include "../PKB/PopulateFacade.h"

#include <vector>

using namespace std;

class DesignExtractor {
protected:
	ProgramNode* program;
	PopulateFacade* storage;

public:
	DesignExtractor(ProgramNode* program);
	DesignExtractor();
	void extractAll();
};

class ProcedureExtractor :public DesignExtractor {
public:
	ProcedureExtractor(ProgramNode* program, PopulateFacade* storage);
	vector<Procedure*> extract();
	void populate();
};

class StatementExtractor :public DesignExtractor {
public:
	StatementExtractor(ProgramNode* program, PopulateFacade* storage);
	vector<Statement*> extract();
	void populate();
};

class VariableExtractor : public DesignExtractor {
public:
	VariableExtractor(ProgramNode* program, PopulateFacade* storage);
	vector<Variable*> extract();
	void populate();
};

class ConstantExtractor :public DesignExtractor {
public:
	ConstantExtractor(ProgramNode* program, PopulateFacade* storage);
	vector<Constant*> extract();
	void populate();
};
