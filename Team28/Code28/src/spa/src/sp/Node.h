#pragma once

#include "Token.h"
#include <vector>

using namespace std;

class Node {
private:
	Token token;
	vector<Node> children;
public:
	Node(Token token);
	void addChildren(Token child);
	vector<Node> getChildren();
	Token getToken();
};


