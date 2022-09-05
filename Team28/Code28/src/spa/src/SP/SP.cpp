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

	cout << "[DEBUG] Done reading from file" << endl;
	vector<Token*> tokens = Tokenizer(buffer.str()).tokenize();
	cout << "[DEBUG] Done tokenizing" << endl;

	for (Token* token: tokens) {
		cout << token->getValue() << endl;	
	}
	cout << "[DEBUG] Start parsing" << endl;
	ProgramNode* program = ProgramParser(0, tokens).parse();
	cout << "[DEBUG] Done parsing" << endl;
	DesignExtractor(program, this->storage).extractAll();
	cout << "[DEBUG] Done extracting" << endl;
}
