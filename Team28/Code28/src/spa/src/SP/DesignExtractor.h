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
	ProgramNode program;
public:
	DesignExtractor(ProgramNode program);
	DesignExtractor();
};

class ProcedureExtractor :public DesignExtractor {
public:
	ProcedureExtractor(ProgramNode program);
	void extract(PopulateFacade storage);
};

class StatementExtractor :public DesignExtractor {
public:
	StatementExtractor(ProgramNode program);
	void extract(PopulateFacade storage);
};

class VariableExtractor : public DesignExtractor {
public:
	VariableExtractor(ProgramNode program);
	void extract(PopulateFacade storage);
};

class ConstantExtractor :public DesignExtractor {
public:
	ConstantExtractor(ProgramNode program);
	void extract(PopulateFacade storage);
};

