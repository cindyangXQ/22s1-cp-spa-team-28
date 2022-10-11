#include "ExprParser.h"
#include "EntityNode.h"
#include <iostream>
#include <typeinfo>
#include <vector>

std::string REL_OP_LIST[] = {"!", ">", "<", "==", "!=", ">=", "<=", "&&", "||"};

ExprParser::ExprParser(int offset, std::vector<Token *> tokens, bool iscond)
    : Parser(offset, tokens) {
    this->iscond = iscond;
}

TermParser::TermParser(int offset, std::vector<Token *> tokens, bool iscond)
    : Parser(offset, tokens) {
    this->iscond = iscond;
}

FactorParser::FactorParser(int offset, std::vector<Token *> tokens, bool iscond)
    : Parser(offset, tokens) {
    this->iscond = iscond;
}

ExpressionNode *CondParser::parse() {
    Token *curr = tokens.at(offset);
    ExpressionNode *root;
    if (curr->getValue() == "!") {
        offset++;
        root = new ExpressionNode(curr);
    }
    ExprParser parser = ExprParser(offset, tokens, true);
    ExpressionNode *result = parser.parse();
    offset = parser.getOffset();
    Token *tempToken = result->getToken();
    if (curr->getValue() == "!") {
        if (std::find(std::begin(REL_OP_LIST), std::end(REL_OP_LIST),
                      tempToken->getValue()) == std::end(REL_OP_LIST)) {
            throw "invalid conditional expression";
        }
        root->setLeft(result);
    } else {
        root = result;
    }

    Token *next = tokens.at(offset);
    while (std::find(std::begin(REL_OP_LIST), std::end(REL_OP_LIST),
                     next->getValue()) != std::end(REL_OP_LIST)) {
        offset++;

        curr = tokens.at(offset);
        if (curr->getValue() == "!") {
            offset++;
            root = new ExpressionNode(curr);
        }
        ExpressionNode *cond = new ExpressionNode(next);
        cond->setLeft(result);

        parser = ExprParser(offset, tokens, true);
        result = parser.parse();
        offset = parser.getOffset();

        cond->setRight(result);

        ExpressionNode *left = cond->getLeft();
        Token *leftToken = left->getToken();
        
        ExpressionNode *right = cond->getRight();
        Token *rightToken = right->getToken();

        if (next->getValue() == "&&" || next->getValue() == "||") {
            if (find(begin(REL_OP_LIST), end(REL_OP_LIST),
                     leftToken->getValue()) == end(REL_OP_LIST) ||
                find(begin(REL_OP_LIST), end(REL_OP_LIST),
                     rightToken->getValue()) == end(REL_OP_LIST)) {
                throw "invalid cond expression";
            }
        } else {
            if (find(begin(REL_OP_LIST), end(REL_OP_LIST),
                     leftToken->getValue()) != end(REL_OP_LIST) ||
                find(begin(REL_OP_LIST), end(REL_OP_LIST),
                     rightToken->getValue()) != end(REL_OP_LIST)) {
                throw "invalid cond expression";
            }
        }

        Token *condToken = cond->getToken();
        if (curr->getValue() == "!") {
            if (find(begin(REL_OP_LIST), end(REL_OP_LIST),
                     condToken->getValue()) == end(REL_OP_LIST)) {
                throw "invalid conditional expression";
            }
            root->setLeft(cond);
            result = root;
        } else {
            result = cond;
            root = cond;
        }

        next = tokens.at(offset);
    }

    if (next->getValue() == ";") {
        offset++;
        return root;
    }
    if (next->getValue() == ")") {
        return root;
    }
}

ExpressionNode *ExprParser::parse() {
    TermParser parser = TermParser(offset, tokens, this->iscond);
    ExpressionNode *result = parser.parse();
    offset = parser.getOffset();
    std::vector<ExpressionNode *> terms;
    terms.push_back(result);
    ExpressionNode *root = result;

    Token *next = tokens.at(offset);
    while (next->getValue() == "+" || next->getValue() == "-") {
        offset++;

        ExpressionNode *expr = new ExpressionNode(next);
        expr->setLeft(terms.back());
        root = expr;

        parser = TermParser(offset, tokens, this->iscond);
        result = parser.parse();
        terms.push_back(result);
        offset = parser.getOffset();

        expr->setRight(terms.back());
        terms.push_back(expr);

        next = tokens.at(offset);
    }

    if (next->getValue() == ";") {
        offset++;
        return root;
    } else if (next->getValue() == ")" || find(begin(REL_OP_LIST), end(REL_OP_LIST),
                                          next->getValue()) != end(REL_OP_LIST)) {
        if (!this->iscond && find(begin(REL_OP_LIST), end(REL_OP_LIST),
                                  next->getValue()) != end(REL_OP_LIST)) {
            throw "invalid expression";
        }
        return root;
    } else {
        throw "invalid expression";
    }
}

ExpressionNode *TermParser::parse() {
    FactorParser parser = FactorParser(offset, tokens, this->iscond);
    ExpressionNode *result = parser.parse();
    std::vector<ExpressionNode *> factors;
    offset = parser.getOffset();
    factors.push_back(result);
    ExpressionNode *root = result;

    Token *next = tokens.at(offset);
    while (next->getValue() == "*" || next->getValue() == "/" || next->getValue() == "%") {
        // continue process as term
        offset++;

        ExpressionNode *term = new ExpressionNode(next);
        term->setLeft(factors.back());

        root = term;

        parser = FactorParser(offset, tokens, this->iscond);
        result = parser.parse();
        factors.push_back(result);
        offset = parser.getOffset();
        term->setRight(factors.back());

        factors.push_back(term);

        next = tokens.at(offset);
    }

    if (next->getValue() == "+" || next->getValue() == "-" || next->getValue() == ";" ||
        next->getValue() == ")" ||
        find(begin(REL_OP_LIST), end(REL_OP_LIST), next->getValue()) !=
            end(REL_OP_LIST)) {
        if (!iscond && find(begin(REL_OP_LIST), end(REL_OP_LIST),
                            next->getValue()) != end(REL_OP_LIST)) {
            throw "invalid expression";
        }
        return root;
    } else {
        throw "invalid expression";
    }
}

ExpressionNode *FactorParser::parse() {
    Token *curr = tokens.at(offset);
    if (curr->isConstant() || curr->isName()) {
        offset++;
        ExpressionNode *result = new ExpressionNode(curr);
        return result;
    } else if (curr->getValue() == "(") {
        offset++;
        ExpressionNode *factor;
        if (iscond) {
            CondParser parser = CondParser(offset, tokens);
            factor = parser.parse();
            offset = parser.getOffset();
        } else {
            ExprParser parser = ExprParser(offset, tokens, false);
            factor = parser.parse();
            offset = parser.getOffset();
        }
        // if next token is not ")" throw error
        if (tokens.at(offset)->getValue() != ")") {
            throw "invalid expression";
        }
        offset++;
        return factor;
    } else {
        throw "invalid expression";
    }
}
