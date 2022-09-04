#include "SP.h"

#include "Token.h"
#include "Tokenizer.h"
#include "EntityNode.h"
#include "Parser.h"
#include "DesignExtractor.h"

#include <fstream>
#include <sstream>

using namespace std;

SP::SP(PopulateFacade* facade) {
	this->storage = facade;
}

void SP::parse(string filename) {
	ifstream t(filename);
	stringstream buffer;
	buffer << t.rdbuf();

	vector<Token*> tokens = Tokenizer(buffer.str()).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	DesignExtractor(program, this->storage).extractAll();
}
