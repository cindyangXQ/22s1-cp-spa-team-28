#include "ExtractUtils.h"

void ExtractUtils::follows(const std::vector<StatementNode*>& stmtList, std::vector<Relationship<int, int>*>& result) {
	std::vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	}
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		Relationship<int, int>* temp = new Relationship<int, int>(RelationshipReference::FOLLOWS, lineNumbers[i - 1], lineNumbers[i]);
		result.push_back(temp);
	}
}

void ExtractUtils::followsT(const std::vector<StatementNode*>& stmtList, std::vector<Relationship<int, int>*>& result) {
	std::vector<int> lineNumbers;
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

bool ExtractUtils::compareStmtList(const std::vector<StatementNode*>& list1, const std::vector<StatementNode*>& list2) {
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
bool ExtractUtils::compareProcList(std::vector<ProcedureNode*>& list1, std::vector<ProcedureNode*>& list2) {
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
