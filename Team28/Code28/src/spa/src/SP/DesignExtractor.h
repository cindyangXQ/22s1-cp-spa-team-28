#include "EntityNode.h"
#include "../commons/Constant.h"
#include "../commons/Procedure.h"
#include "../commons/Variable.h"
#include "../commons/Statement.h"
#include "../PKB/PopulateFacade.h"

#include <vector>

using namespace std;

/*
class DesignExtractor {
	ProgramNode program;

public:
	DesignExtractor(ProgramNode program);

	vector<string> extractProcedure();
	vector<string> extractStatement();
	vector<string> extractReadStmt();
	vector<string> extractPrintStmt();
	vector<string> extractCallStmt();
	vector<string> extractAssignStmt();
	vector<string> extractVariable();
	vector<string> extractConstant();

	vector<ModifyRel> extractModifies();
};*/

class DesignExtractor {
protected:
	ProgramNode* program;
public:
	DesignExtractor(ProgramNode* program);
	DesignExtractor();
};

class StatementExtractor :public DesignExtractor {
public:
	StatementExtractor(ProgramNode* program);
	vector<Statement*> extract(PopulateFacade storage);
};

class VariableExtractor : public DesignExtractor {
public:
	VariableExtractor(ProgramNode* program);
	vector<Variable*> extract(PopulateFacade storage);
};


class ProcedureExtractor :public DesignExtractor {
public:
	ProcedureExtractor(ProgramNode* program);
	vector<Procedure*> extract(PopulateFacade storage);
};

class ConstantExtractor :public DesignExtractor {
public:
	ConstantExtractor(ProgramNode* program);
	vector<Constant*> extract(PopulateFacade storage);
};

