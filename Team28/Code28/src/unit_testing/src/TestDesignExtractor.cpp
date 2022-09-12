#include "catch.hpp"
#include "SP/DesignExtractor.h"
#include "SP/ExtractUtils.h"

#include "SP/Tokenizer.h"
#include "SP/Parser.h"

TEST_CASE("extract procedure small program") {
	vector<Procedure*> expected;
	expected.push_back(new Procedure("Bedok"));

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	ProcedureExtractor extr(program, NULL);
	vector<Procedure*> extracted = extr.extract();
	
	REQUIRE(expected.size() == extracted.size());
	for (int i = 0; i < expected.size(); i++) {
		REQUIRE(expected[i]->getName() == extracted[i]->getName());
	}
}

TEST_CASE("extract statement small program") {
	vector<Statement*> expected;
	expected.push_back(new Statement(1, StatementType::ASSIGN));
	expected.push_back(new Statement(2, StatementType::ASSIGN));
	expected.push_back(new Statement(3, StatementType::ASSIGN));
	expected.push_back(new Statement(4, StatementType::ASSIGN));

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	StatementExtractor extr(program, NULL);
	vector<Statement*> extracted = extr.extract();

	REQUIRE(expected.size() == extracted.size());
	for (int i = 0; i < expected.size(); i++) {
		REQUIRE(expected[i]->isLineNumberEqual(extracted[i]));
		REQUIRE(expected[i]->isStatementTypeEqual(extracted[i]));
	}
}

TEST_CASE("extract follows small program") {
	vector<Relationship<int, int>*> expected;
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS, 1, 2));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS, 2, 3));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS, 3, 4));

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	FollowsExtractor extr(program, NULL);
	vector<Relationship<int, int>*> extracted = extr.extract();

	REQUIRE(expected.size() == extracted.size());
	for (int i = 0; i < expected.size(); i++) {
		printf("left %d right %d\n", extracted[i]->getLeft(), extracted[i]->getRight());
		/*REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
		REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
		REQUIRE(expected[i]->getRelationshipReference() == extracted[i]->getRelationshipReference());*/
	}
}
