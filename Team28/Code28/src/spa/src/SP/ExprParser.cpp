#include "ExprParser.h"
#include "EntityNode.h"
#include <vector>
#include <typeinfo>
#include <iostream>

using namespace std;

ExprParser::ExprParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

TermParser::TermParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

FactorParser::FactorParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ParseResult<ExpressionNode> ExprParser::parse() {
	int index = this->offset;
	cout << tokens.at(1).isName() <<endl;
	TermParser parser = TermParser(index, tokens);
	ParseResult result = parser.parse();
	index = result.index;
	vector<ExpressionNode> terms;
	terms.push_back(result.entity);

	index++;
	Token next = tokens.at(index);
	while (next.value == "+" || next.value == "-") {
		index++;

		ExpressionNode expr = ExpressionNode(next);
		expr.left = &terms.back();

		result = TermParser(index, tokens).parse();
		terms.push_back(result.entity);
		index = result.index;

		expr.right = &terms.back();
		terms.push_back(expr);

		index++;
		next = tokens.at(index);
	}

	if (next.value == ";") {
		result.index = index + 1;
		return result;
	}
	else {
		//throw error
	}
}


ParseResult<ExpressionNode> TermParser::parse() {
	int index = offset;
	FactorParser parser = FactorParser(offset, tokens);
	ParseResult result = parser.parse();
	vector<ExpressionNode> factors;
	index = result.index;
	factors.push_back(result.entity);

	index++;
	Token next = tokens.at(index);
	while (next.value == "*" || next.value == "/" || next.value == "%") {
		//continue process as term
		index++;

		ExpressionNode term = ExpressionNode(next);
		term.left = &factors.back();

		result = FactorParser(index, tokens).parse();
		factors.push_back(result.entity);
		index = result.index;
		term.right = &factors.back();

		factors.push_back(term);

		index++;
		next = tokens.at(index);
	}

	if (next.value == "+" || next.value == "-" || next.value == ";") {
		// term end, return to expression
		return result;
	}
	else {
		//throw error
	}
}

ParseResult<ExpressionNode> FactorParser::parse() {
	int index = this->offset;
	Token curr = tokens.at(offset);
	if (typeid(curr) == typeid(ConstantNode) || typeid(curr) == typeid(VariableNode)) {
		ParseResult<ExpressionNode> result = { ExpressionNode(curr), offset };
		return result;
	}
	else if (curr.value == "(") {
		index++;
		ExprParser parser = ExprParser(index, tokens);
		ParseResult<ExpressionNode> factor = parser.parse();
		//if next token is not ")" throw error
		factor.index++;
		return factor;
	}
	else {
		//throw error
	}
}
