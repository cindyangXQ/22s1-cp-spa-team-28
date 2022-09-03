#include "EntityNode.h"
#include "../commons/Constant.h"
#include "../commons/Procedure.h"
#include "../commons/Variable.h"
#include "../commons/Statement.h"

#include <vector>

using namespace std;

class DesignExtractor {
protected:
	ProgramNode program;
public:
	DesignExtractor(ProgramNode program);
	DesignExtractor();
};

class StatementExtractor :public DesignExtractor {
public:
	StatementExtractor(ProgramNode program);
	vector<Statement> extract();
};

class VariableExtractor : public DesignExtractor {
public:
	VariableExtractor(ProgramNode program);
	vector<Variable> extract();
};


class ProcedureExtractor :public DesignExtractor {
public:
	ProcedureExtractor(ProgramNode program);
	vector<Procedure> extract();
};

class ConstantExtractor :public DesignExtractor {
public:
	ConstantExtractor(ProgramNode program);
	vector<Constant> extract();
};

