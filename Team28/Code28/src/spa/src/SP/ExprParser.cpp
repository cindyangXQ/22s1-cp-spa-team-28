#include "ExprParser.h"
#include "EntityNode.h"
#include <vector>
#include <typeinfo>
#include <iostream>

using namespace std;

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

ExpressionNode* ExprParser::parse() {
	//cout << "______________________" << endl;
	TermParser parser = TermParser(offset, tokens);
	ExpressionNode* result = parser.parse();
	offset = parser.getOffset();
	vector<ExpressionNode*> terms;
	terms.push_back(result);

	Token* next = tokens.at(offset);
	while (next->value == "+" || next->value == "-") {
		offset++;

		ExpressionNode* expr = new ExpressionNode(next);
		cout << next->value << endl;
		expr->left = terms.back();

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
		return result;
	}
	else if (next->value == ")") {
		//cout << result.index << endl;
		return result;
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

	offset++;
	Token* next = tokens.at(offset);
	while (next->value == "*" || next->value == "/" || next->value == "%") {
		//continue process as term
		offset++;

		ExpressionNode* term = new ExpressionNode(next);
		cout << next->value << endl;
		term->left = factors.back();

		parser = FactorParser(offset, tokens);
		result = parser.parse();
		factors.push_back(result);
		offset = parser.getOffset();
		term->right = factors.back();

		factors.push_back(term);

		offset++;
		next = tokens.at(offset);
	}

	if (next->value == "+" || next->value == "-" || next->value == ";"||next->value == ")") {
		// term end, return to expression
		return result;
	}
	else {
		//throw error
	}
}

ExpressionNode* FactorParser::parse() {
	Token* curr = tokens.at(offset);
	if (curr->isConstant() || curr->isName()) {
		cout << curr->value << endl;
		ExpressionNode* result = new ExpressionNode(curr);
		return result;
	}
	else if (curr->value == "(") {
		offset++;
		ExprParser parser = ExprParser(offset, tokens);
		ExpressionNode* factor = parser.parse();
		//if next token is not ")" throw error
		offset++;
		return factor;
	}
	else {
		//throw error
	}
}
