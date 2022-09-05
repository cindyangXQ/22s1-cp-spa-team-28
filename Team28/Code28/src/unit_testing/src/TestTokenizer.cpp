#include "catch.hpp"
#include "SP/Tokenizer.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE() {
	string sourceProgram = "procedure Bedok {\nx2 = (2 * 1 + 2) / 3;\n}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token*> token_list = tokenizer.tokenize();

	for (int i = 0; i < token_list.size(); i++) {
		Token* temp = token_list[i];
		std::cout << temp->value << endl;
	}

	ProgramParser parser = ProgramParser(0, token_list);
	ProgramNode* program = parser.parse();
	std::cout << program->getProcList().size() << endl;
	//ProcedureNode* procedure = program->getProcList().at(0);
	Storage* storage = new Storage();
	PopulateFacade facade = PopulateFacade(storage);
	//DesignExtractor(program, &facade).extractAll();
	vector<Variable*> vars = VariableExtractor(program, &facade).extract();

	
	std::cout << "__________________________" << endl;
	for (size_t i = 0; i < vars.size(); i++) {
		std::cout << vars.at(i)->getName();
	}
}

string small_expected[] = { "procedure", "Bedok", "{", "west", "=", "9", "+", "east", ";",
		"y", "=", "east", "-", "4", ";", "z", "=", "west", "+", "2", ";", "west", "=", "9", "+", "east", "+", "west", ";" };

TEST_CASE("Tokenize a normal spacing small program") {
	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest= 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected.size(); i++) {
		assert(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize a less spacing small program") {
	string sourceProgram = "procedure Bedok {\nwest=9+east;\ny=east-4;\nz=west+2;\nwest=9+east+west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected.size(); i++) {
		assert(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize a least spacing small program") {
	string sourceProgram = "procedure Bedok{west=9+east;y=east-4;z=west+2;west=9+east+west;}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected.size(); i++) {
		assert(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize an extra spacing small program") {
	string sourceProgram = "procedure   Bedok \t { \n  west\t= \t9 +    east  ;\n   y =   east - 4 ; z = west +   2   \t;  west = 9 + east + west  ; \n}\n";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected.size(); i++) {
		assert(tokens[i]->equals(small_expected[i]));
	}
}
