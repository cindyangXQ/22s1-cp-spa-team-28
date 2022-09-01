#include "Node.h"
#include "Token.h"
#include <vector>

using namespace std;

Node::Node(Token token) {
	this->token = token;
	children = {};
}

void Node::addChildren(Token child) {
	children.push_back(child);
}

vector<Node> Node::getChildren() {
	return this->children;
}

Token Node::getToken() {
	return this->token;
}
