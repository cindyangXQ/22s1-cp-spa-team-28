#include "EntityNode.h"

class ExtractUtils {
public:
	static void follows(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void followsT(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	static void parent(StatementNode* parentNode, vector<Relationship<int, int>*>& result);
	static vector<int>* parentT(StatementNode* parentNode, vector<Relationship<int, int>*>& result);

	//static void usesS(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);
	//static void ModifiesS(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result);

	static bool compareStmtList(const vector<StatementNode*>& list1, const vector<StatementNode*>& list2);
	static bool compareProcList(vector<ProcedureNode*>& list1, vector<ProcedureNode*>& list2);
};