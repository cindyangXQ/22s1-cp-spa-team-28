#include "EntityNode.h"

class ExtractUtils {
public:
	static void follows(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void followsT(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);

	static bool compareStmtList(vector<StatementNode*>& list1, vector<StatementNode*>& list2);
	static bool compareProcList(vector<ProcedureNode*>& list1, vector<ProcedureNode*>& list2);
};