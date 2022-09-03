#include "ExprParser.h"
#include "Token.h"
#include "EntityNode.h"
#include <vector>
#include <typeinfo>

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

ParseResult ExprParser::parse() {
	TermParser parser = TermParser(offset, tokens);
	ExpressionNode term = parser.parse();

	Token next = tokens.at(offset + 1);
	while (next.value == "+" || next.value == "-") {
		offset++;

		ExpressionNode expr = Node(next);
		expr.addChildren(expr);

		term = Factor(&offset, tokens).parse();
		expr.addChildren(term);

		term = expr;
		next = tokens.at(offset + 1)
	}

	if (next.value == ";") {
		return term;
	}
	else {
		//throw error
	}
}


ParseResult TermParser::parse() {
	FactorParser parser = FactorParser(offset, tokens);
	Node factor = parser.parse();

	Token next = tokens.at(offset + 1);
	while (next.value == "*" || next.value == "/" || next.value == '%') {
		//continue process as term
		offset++;

		Node term = Node(next);
		term.addChildren(factor);

		//second factor
		factor = FactorParser(&offset, tokens).parse();
		term.addChildren(factor);

		factor = term;
		next = tokens.at(offset + 1)
	}

	if (next.value == "+" || next.value == "-" || next.value == ";") {
		// term end, return to expression
		return factor;
	}
	else {
		//throw error
	}
}

ParseResult FactorParser::parse() {
	Token curr = tokens.at(offset);
	if (typeid(curr) == typeid(Constant) || typeid(curr) == typeid(Name)) {
		return Node(curr);
	}
	else if (curr.value == "(") {
		offset++;
		ExprParser parser = ExprParser(&offset, tokens);
		Node factor = parser.parse();
		//if next token is not ")" throw error
		offset++;
		return factor;
	}
	else {
		//throw error
	}
}
