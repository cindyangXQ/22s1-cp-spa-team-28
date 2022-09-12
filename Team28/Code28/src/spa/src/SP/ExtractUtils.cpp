#include "ExtractUtils.h"

void ExtractUtils::follows(vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result) {
	vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	}
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		Relationship<int, int> temp(RelationshipReference::FOLLOWS, lineNumbers[i - 1], lineNumbers[i]);
		result.push_back(&temp);
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
			Relationship<int, int> temp(RelationshipReference::FOLLOWS_T, lineNumbers[j], lineNumbers[i]);
			result.push_back(&temp);
		}
		
	}
}
