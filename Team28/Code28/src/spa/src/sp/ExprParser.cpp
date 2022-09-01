#include "ExprParser.h"
#include "Token.h"
#include "Node.h"
#include <vector>
#include <typeinfo>

using namespace std;

Node ExprParser::parse() {
	TermParser parser = TermParser(&offset, tokens);
	Node term = parser.parse();

	Token next = tokens.at(offset + 1);
	while (next.value == "+" || next.value == "-") {
		offset++;

		Node expr = Node(next);
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


Node TermParser::parse() {
	FactorParser parser = FactorParser(&offset, tokens);
	Node factor = parser.parse();
	
	Token next = tokens.at(offset + 1);
	while (next.value == "*" || next.value == "/" || next.value == '%') {
		//continue process as term
		offset++;

		Node term = Node(next);
		term.addChildren(factor);

		//second factor
		factor = Factor(&offset, tokens).parse();
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

Node FactorParser::parse() {
	Token curr = tokens.at(offset);
	if (typeid(curr) == typeid(Constant) || typeid(curr) == typeid(Name)) {
		return Node(curr);
	}
	else if (curr.value == "("){
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
