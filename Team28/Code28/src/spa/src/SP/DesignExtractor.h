#include "EntityNode.h"
#include "Relationship.h"
#include <vector>

using namespace std;

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
};
