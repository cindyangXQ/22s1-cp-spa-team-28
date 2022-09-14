#include "catch.hpp"
#include "SP/Tokenizer.h"

#include <iostream>
#include <vector>

using namespace std;

string small_expected[] = { "procedure", "Bedok", "{", "west", "=", "9", "+", "east", ";",
		"y", "=", "east", "-", "4", ";", "z", "=", "west", "+", "2", ";", "west", "=", "9", "+", "east", "+", "west", ";" };

TEST_CASE("Tokenize a normal spacing small program") {
	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest= 9 + east + west;\n}";
	Tokenizer tokenizer = Tokenizer(sourceProgram);
	vector<Token*> tokens = tokenizer.tokenize();
	for (int i = 0; i < small_expected->size(); i++) {
		REQUIRE(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize a less spacing small program") {
	string sourceProgram = "procedure Bedok {\nwest=9+east;\ny=east-4;\nz=west+2;\nwest=9+east+west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected->size(); i++) {
		REQUIRE(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize a least spacing small program") {
	string sourceProgram = "procedure Bedok{west=9+east;y=east-4;z=west+2;west=9+east+west;}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected->size(); i++) {
		REQUIRE(tokens[i]->equals(small_expected[i]));
	}
}

TEST_CASE("Tokenize an extra spacing small program") {
	string sourceProgram = "procedure   Bedok \t { \n  west\t= \t9 +    east  ;\n   y =   east - 4 ; z = west +   2   \t;  west = 9 + east + west  ; \n}\n";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	for (int i = 0; i < small_expected->size(); i++) {
		REQUIRE(tokens[i]->equals(small_expected[i]));

	}
}
