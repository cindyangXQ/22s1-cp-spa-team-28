#include "EntityNode.h"

class ExtractUtils {
public:
	static void follows(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void followsT(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void parent(StatementNode* parentNode, vector<Relationship<int, int>*>& result);
	static vector<int>* parentT(StatementNode* parentNode, vector<Relationship<int, int>*>& result);

	static bool compareStmtList(const std::vector<StatementNode*>& list1, const std::vector<StatementNode*>& list2);
	static bool compareProcList(std::vector<ProcedureNode*>& list1, std::vector<ProcedureNode*>& list2);
};
