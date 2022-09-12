#include "EntityNode.h"

class ExtractUtils {
public:
	static void follows(vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void followsT(vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
};