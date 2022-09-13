#include "ExtractUtils.h"

void ExtractUtils::follows(vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result) {
	vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	}
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		Relationship<int, int>* temp = new Relationship<int, int>(RelationshipReference::FOLLOWS, lineNumbers[i - 1], lineNumbers[i]);
		printf("create follows(%d, %d)\n", temp->getLeft(), temp->getRight());
		result.push_back(temp);
	}

	for (size_t i = 1; i < result.size(); i++) {
		printf("check util result(%d, %d)\n", result[i]->getLeft(), result[i]->getRight());
	}
}

void ExtractUtils::followsT(vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result) {
	vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsTInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	}
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		for (size_t j = 0; j < i; j++) {
			Relationship<int, int>* temp = new Relationship<int,int>(RelationshipReference::FOLLOWS_T, lineNumbers[j], lineNumbers[i]);
			result.push_back(temp);
		}
		
	}
}

bool ExtractUtils::compareStmtList(vector<StatementNode*>& list1, vector<StatementNode*>& list2) {
	if (list1.size() != list2.size()) {
		return false;
	}
	for (int i = 0; i < list1.size(); i++) {
		if (!list1[i]->equals(list2[i])) {
			return false;
		}
	}
	return true;
}
//DRY
bool ExtractUtils::compareProcList(vector<ProcedureNode*>& list1, vector<ProcedureNode*>& list2) {
	if (list1.size() != list2.size()) {
		return false;
	}
	for (int i = 0; i < list1.size(); i++) {
		if (!list1[i]->equals(list2[i])) {
			return false;
		}
	}
	return true;
}
