#include "ExtractUtils.h"

void ExtractUtils::follows(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result) {
	vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	}
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		Relationship<int, int>* temp = new Relationship<int, int>(
			RelationshipReference::FOLLOWS, lineNumbers[i - 1], lineNumbers[i]);
		result.push_back(temp);
	}
}

void ExtractUtils::followsT(const vector<StatementNode*>& stmtList, vector<Relationship<int, int>*>& result) {
	vector<int> lineNumbers;
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList[i]->getFollowsTInto(result);
		lineNumbers.push_back(stmtList[i]->getLineNumber());
	} 
	for (size_t i = 1; i < lineNumbers.size(); i++) {
		for (size_t j = 0; j < i; j++) {
			Relationship<int, int>* temp = new Relationship<int,int>(
				RelationshipReference::FOLLOWS_T, lineNumbers[j], lineNumbers[i]);
			result.push_back(temp);
		}
		
	}
}

void ExtractUtils::parent(StatementNode* parentNode, vector<Relationship<int, int>*>& result) {
	int parentLineNo = parentNode->getLineNumber();
	vector<StatementNode*> children = parentNode->getStmtList();

	for (size_t i = 0; i < children.size(); i++) {
		ExtractUtils::parentT(children[i], result);
		int child = children[i]->getLineNumber();
		Relationship<int, int>* addChild = new Relationship<int, int>(
			RelationshipReference::PARENT, parentLineNo, child);
		result.push_back(addChild);
	}
}

vector<int>* ExtractUtils::parentT(StatementNode* parentNode, vector<Relationship<int, int>*>& result) {
	int parentLineNo = parentNode->getLineNumber();
	vector<StatementNode*> children = parentNode->getStmtList();
	vector<int> descendants;

	for (size_t i = 0; i < children.size(); i++) {
		vector<int>* grandchildren = ExtractUtils::parentT(children[i], result);
		for (size_t j = 0; j < grandchildren->size(); j++) {
			int grandchild = grandchildren->at(j);
			Relationship<int, int>* addGrandchild = new Relationship<int, int>(
				RelationshipReference::PARENT_T, parentLineNo, grandchild);
			result.push_back(addGrandchild);
			descendants.push_back(grandchild);
		}

		int child = children[i]->getLineNumber();
		Relationship<int, int>* addChild = new Relationship<int, int>(
			RelationshipReference::PARENT_T, parentLineNo, child);
		result.push_back(addChild);
		descendants.push_back(child);
	}

	return &descendants;
}



bool ExtractUtils::compareStmtList(const vector<StatementNode*>& list1, const vector<StatementNode*>& list2) {
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
