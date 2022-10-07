#include "SP.h"

#include "DesignExtractor.h"
#include "EntityNode.h"
#include "ExprParser.h"
#include "Parser.h"
#include "Token.h"
#include "Tokenizer.h"

#include <fstream>
#include <sstream>

SP::SP(PopulateFacade *facade) { this->storage = facade; }

void SP::parse(std::string filename) {
    try {
        std::ifstream t(filename);
        std::stringstream buffer;
        buffer << t.rdbuf();

        std::vector<Token *> tokens = Tokenizer(buffer.str()).tokenize();

        ProgramNode *program = ProgramParser(0, tokens).parse();
        DesignExtractor(program, this->storage).extractAll();
    } catch (...) {
        std::cout << "Parsing failed. Exiting program." << std::endl;
        exit(1);
    }
}

std::string SP::convertExpression(std::string input) {
    Tokenizer tokenizer(input + ";");
    std::vector<Token *> tokens = tokenizer.tokenize();
    ExprParser parser(0, tokens, false);
    ExpressionNode *expr = parser.parse();
    if (parser.getOffset() != tokens.size()) {
        throw "expression syntax error";
    }
    return expr->toString();
}
