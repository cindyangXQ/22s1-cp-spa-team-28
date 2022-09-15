#include "EntityNode.h"

class ExtractUtils {
public:
	static void follows(const std::vector<StatementNode*>& stmtList, std::vector<Relationship<int, int>*>& result);
	static void followsT(const std::vector<StatementNode*>& stmtList, std::vector<Relationship<int, int>*>& result);

	static bool compareStmtList(const std::vector<StatementNode*>& list1, const std::vector<StatementNode*>& list2);
	static bool compareProcList(std::vector<ProcedureNode*>& list1, std::vector<ProcedureNode*>& list2);
};
