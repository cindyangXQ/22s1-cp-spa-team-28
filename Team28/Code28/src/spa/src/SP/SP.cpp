#include "SP.h"

#include "DesignExtractor.h"
#include "EntityNode.h"
#include "Parser.h"
#include "Token.h"
#include "Tokenizer.h"

#include <fstream>
#include <sstream>

SP::SP(PopulateFacade *facade) { this->storage = facade; }

void SP::parse(std::string filename) {
    std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();

    std::vector<Token *> tokens = Tokenizer(buffer.str()).tokenize();

    ProgramNode *program = ProgramParser(0, tokens).parse();
    DesignExtractor(program, this->storage).extractAll();
}
