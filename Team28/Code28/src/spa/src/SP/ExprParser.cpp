#include "ExprParser.h"
#include "EntityNode.h"
#include <vector>
#include <typeinfo>
#include <iostream>

using namespace std;

string REL_OP_LIST[] = { ">", "<", "==", "!=", ">=", "<=","&&", "||"};

CondParser::CondParser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ExprParser::ExprParser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

TermParser::TermParser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

FactorParser::FactorParser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ExpressionNode* CondParser::parse() {
	Token* curr = tokens.at(offset);
	ExpressionNode* root;
	if (curr->value == "!") {
		offset++;
		root = new ExpressionNode(curr);
	}
	ExprParser parser = ExprParser(offset, tokens);
	ExpressionNode* result = parser.parse();
	offset = parser.getOffset();
	if (curr -> value == "!") {
		root->left = result;
	}
	else {
		root = result;
	}

	Token* next = tokens.at(offset);
	while (find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST) || next->value == "&&" || next->value == "||") {
		offset++;

		curr = tokens.at(offset);
		if (curr->value == "!") {
			offset++;
			root = new ExpressionNode(curr);
		}
		ExpressionNode* cond = new ExpressionNode(next);
		cond->left = result;

		parser = ExprParser(offset, tokens);
		result = parser.parse();
		offset = parser.getOffset();

		cond->right = result;

		if (curr->value == "!") {
			root->left = cond;
			result = root;
		}
		else {
			result = cond;
			root = cond;
		}

		next = tokens.at(offset);
	}
	if (next->value == ";") {
		offset++;
		return root;
	}
	if (next->value == ")") {
		return root;
	}
}

ExpressionNode* ExprParser::parse() {
	//cout << "______________________" << endl;
	TermParser parser = TermParser(offset, tokens);
	ExpressionNode* result = parser.parse();
	offset = parser.getOffset();
	vector<ExpressionNode*> terms;
	terms.push_back(result);
	ExpressionNode* root = result;

	Token* next = tokens.at(offset);
	while (next->value == "+" || next->value == "-") {
		offset++;

		ExpressionNode* expr = new ExpressionNode(next);
		cout << next->value << endl;
		expr->left = terms.back();
		root = expr;

		parser = TermParser(offset, tokens);
		result = parser.parse();
		terms.push_back(result);
		offset = parser.getOffset();

		expr->right = terms.back();
		terms.push_back(expr);

		next = tokens.at(offset);
	}
	
	if (next->value == ";") {
		offset ++;
		return root;
	}
	else if (next->value == ")" || find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST)) {
		//cout << result.index << endl;
		return root;
	}
	else {
		//throw error
	}
}


ExpressionNode* TermParser::parse() {
	FactorParser parser = FactorParser(offset, tokens);
	ExpressionNode* result = parser.parse();
	vector<ExpressionNode*> factors;
	offset = parser.getOffset();
	factors.push_back(result);
	ExpressionNode* root = result;

	Token* next = tokens.at(offset);
	while (next->value == "*" || next->value == "/" || next->value == "%") {
		//continue process as term
		offset++;

		ExpressionNode* term = new ExpressionNode(next);
		cout << next->value << endl;
		term->left = factors.back();
		root = term;

		parser = FactorParser(offset, tokens);
		result = parser.parse();
		factors.push_back(result);
		offset = parser.getOffset();
		term->right = factors.back();

		factors.push_back(term);

		next = tokens.at(offset);
	}

	if (next->value == "+" || next->value == "-" || next->value == ";"||next->value == ")"|| find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST)) {
		// term end, return to expression
		return root;
	}
	else {
		//throw error
	}
}

ExpressionNode* FactorParser::parse() {
	Token* curr = tokens.at(offset);
	if (curr->isConstant() || curr->isName()) {
		offset++;
		cout << curr->value << endl;
		ExpressionNode* result = new ExpressionNode(curr);
		return result;
	}
	else if (curr->value == "(") {
		offset++;
		CondParser parser = CondParser(offset, tokens);
		ExpressionNode* factor = parser.parse();
		offset = parser.getOffset();
		//if next token is not ")" throw error
		offset++;
		return factor;
	}
	else {
		//throw error
	}
}
