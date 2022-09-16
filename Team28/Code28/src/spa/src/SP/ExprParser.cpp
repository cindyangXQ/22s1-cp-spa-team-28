#include "ExprParser.h"
#include "EntityNode.h"
#include <vector>
#include <typeinfo>
#include <iostream>

std::string REL_OP_LIST[] = { "!", ">", "<", "==", "!=", ">=", "<=","&&", "||" };

ExprParser::ExprParser(int offset, std::vector<Token*> tokens, bool iscond) : Parser(offset, tokens) {
	this->iscond = iscond;
}

TermParser::TermParser(int offset, std::vector<Token*> tokens, bool iscond) : Parser(offset, tokens) {
	this->iscond = iscond;
}

FactorParser::FactorParser(int offset, std::vector<Token*> tokens, bool iscond) : Parser(offset, tokens) {
	this->iscond = iscond;
}

ExpressionNode* CondParser::parse() {
	Token* curr = tokens.at(offset);
	ExpressionNode* root;
	if (curr->value == "!") {
		offset++;
		root = new ExpressionNode(curr);
	}
	ExprParser parser = ExprParser(offset, tokens, true);
	ExpressionNode* result = parser.parse();
	offset = parser.getOffset();
	if (curr -> value == "!") {
		if (std::find(std::begin(REL_OP_LIST), std::end(REL_OP_LIST), result->getToken()->value) == std::end(REL_OP_LIST)) {
			throw "invalid conditional expression";
		}
		root->left = result;
	}
	else {
		root = result;
	}

	Token* next = tokens.at(offset);
	while (std::find(std::begin(REL_OP_LIST), std::end(REL_OP_LIST), next->value) != std::end(REL_OP_LIST)) {
		offset++;

		curr = tokens.at(offset);
		if (curr->value == "!") {
			offset++;
			root = new ExpressionNode(curr);
		}
		ExpressionNode* cond = new ExpressionNode(next);
		cond->left = result;

		parser = ExprParser(offset, tokens, true);
		result = parser.parse();
		offset = parser.getOffset();

		cond->right = result;

		if (next->value == "&&" || next->value == "||") {
			if (find(begin(REL_OP_LIST), end(REL_OP_LIST), cond->left->getToken()->value) == end(REL_OP_LIST)
				|| find(begin(REL_OP_LIST), end(REL_OP_LIST), cond->right->getToken()->value) == end(REL_OP_LIST)) {
				std::cout << cond->left->getToken()->value << " " << cond->right->getToken()->value;
				throw "invalid cond expression";
			}
		}
		else {
			if (find(begin(REL_OP_LIST), end(REL_OP_LIST), cond->left->getToken()->value) != end(REL_OP_LIST)
				|| find(begin(REL_OP_LIST), end(REL_OP_LIST), cond->right->getToken()->value) != end(REL_OP_LIST)) {
				std::cout << cond->left->getToken()->value << " " << cond->right->getToken()->value;
				throw "invalid cond expression";
			}
		}

		if (curr->value == "!") {
			if (find(begin(REL_OP_LIST), end(REL_OP_LIST), cond->getToken()->value) == end(REL_OP_LIST)) {
				throw "invalid conditional expression";
			}
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
	TermParser parser = TermParser(offset, tokens, this->iscond);
	ExpressionNode* result = parser.parse();
	offset = parser.getOffset();
	std::vector<ExpressionNode*> terms;
	terms.push_back(result);
	ExpressionNode* root = result;

	Token* next = tokens.at(offset);
	while (next->value == "+" || next->value == "-") {
		offset++;

		ExpressionNode* expr = new ExpressionNode(next);
		expr->left = terms.back();
		root = expr;

		parser = TermParser(offset, tokens, this->iscond);
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
		if (!this->iscond && find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST)) {
			throw "invalid expression";
		}
		return root;
	}
	else {
		throw "invalid expression";
	}
}


ExpressionNode* TermParser::parse() {
	FactorParser parser = FactorParser(offset, tokens, this->iscond);
	ExpressionNode* result = parser.parse();
	std::vector<ExpressionNode*> factors;
	offset = parser.getOffset();
	factors.push_back(result);
	ExpressionNode* root = result;

	Token* next = tokens.at(offset);
	while (next->value == "*" || next->value == "/" || next->value == "%") {
		//continue process as term
		offset++;

		ExpressionNode* term = new ExpressionNode(next);
		term->left = factors.back();
		root = term;

		parser = FactorParser(offset, tokens, this->iscond);
		result = parser.parse();
		factors.push_back(result);
		offset = parser.getOffset();
		term->right = factors.back();

		factors.push_back(term);

		next = tokens.at(offset);
	}

	if (next->value == "+" || next->value == "-" || next->value == ";"||next->value == ")"|| find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST)) {
		if (!iscond && find(begin(REL_OP_LIST), end(REL_OP_LIST), next->value) != end(REL_OP_LIST)) {
			throw "invalid expression";
		}
		return root;
	}
	else {
		throw "invalid expression";
	}
}

ExpressionNode* FactorParser::parse() {
	Token* curr = tokens.at(offset);
	if (curr->isConstant() || curr->isName()) {
		offset++;
		ExpressionNode* result = new ExpressionNode(curr);
		return result;
	}
	else if (curr->value == "(") {
		offset++;
		ExpressionNode* factor;
		if (iscond) {
			CondParser parser = CondParser(offset, tokens);
			factor = parser.parse();
			offset = parser.getOffset();
		}
		else {
			ExprParser parser = ExprParser(offset, tokens, false);
			factor = parser.parse();
			offset = parser.getOffset();
		}
		//if next token is not ")" throw error
		if (tokens.at(offset)->value != ")") {
			throw "invalid expression";
		}
		offset++;
		return factor;
	}
	else {
		throw "invalid expression";
	}
}
