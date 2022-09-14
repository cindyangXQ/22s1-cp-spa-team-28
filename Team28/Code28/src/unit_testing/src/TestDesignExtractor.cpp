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
		REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
		REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
		REQUIRE(expected[i]->getRelationshipReference() == extracted[i]->getRelationshipReference());
	}
}

TEST_CASE("extract followsT small program") {
	vector<Relationship<int, int>*> expected;
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 1, 2));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 1, 3));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 2, 3));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 1, 4));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 2, 4));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 3, 4));

	string sourceProgram = "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + 2;\nwest = 9 + east + west;\n}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramNode* program = ProgramParser(0, tokens).parse();
	FollowsExtrT extr(program, NULL);
	vector<Relationship<int, int>*> extracted = extr.extract();

	REQUIRE(expected.size() == extracted.size());
	for (int i = 0; i < expected.size(); i++) {
		REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
		REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
		REQUIRE(expected[i]->getRelationshipReference() == extracted[i]->getRelationshipReference());
	}
}

TEST_CASE("Extract program with if else statements") {
	// Follow relationship
	vector<Relationship<int, int>*> expected;
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 3, 4));
	expected.push_back(new Relationship(RelationshipReference::FOLLOWS_T, 1, 2));
	
	string sourceProgram = "procedure Bedok{print c; if(a*b!=4) then {a=3; while(a*b!=3){a=2;}} else{read c;}}";
	vector<Token*> tokens = Tokenizer(sourceProgram).tokenize();
	ProgramParser parser = ProgramParser(0, tokens);
	ProgramNode* program = parser.parse();
	FollowsExtrT extr(program, NULL);
	vector<Relationship<int, int>*> extracted = extr.extract();

	REQUIRE(expected.size() == extracted.size());
	for (int i = 0; i < expected.size(); i++) {
		REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
		REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
		REQUIRE(expected[i]->getRelationshipReference() == extracted[i]->getRelationshipReference());
	}

	// Variable extr
	vector<string> varexpected = { "a", "b", "c"};
	vector<Variable*> varresult = VariableExtractor(program, NULL).extract();

	REQUIRE(varexpected.size() == varresult.size());
	for (int i = 0; i < varresult.size(); i++) {
		REQUIRE(find(begin(varexpected), end(varexpected), varresult.at(i)->getName()) != end(varexpected));
	}

	// Constant extr
	vector<string> constexpected = { "2", "3", "4"};
	ConstantExtractor extractor(program, NULL);
	vector<Constant*> constresult = extractor.extract();

	REQUIRE(constexpected.size() == constresult.size());

	for (int i = 0; i < constresult.size(); i++) {
		REQUIRE(find(begin(constexpected), end(constexpected), constresult.at(i)->getName()) != end(constexpected));
	}

	// Statement exty
	vector<Statement*> stmtExpected;
	stmtExpected.push_back(new Statement(1, StatementType::PRINT));
	stmtExpected.push_back(new Statement(2, StatementType::IF));
	stmtExpected.push_back(new Statement(3, StatementType::ASSIGN));
	stmtExpected.push_back(new Statement(4, StatementType::WHILE));
	stmtExpected.push_back(new Statement(5, StatementType::ASSIGN));
	stmtExpected.push_back(new Statement(6, StatementType::READ));

	vector<Statement*> stmtExtracted = StatementExtractor(program, NULL).extract();
	REQUIRE(stmtExpected.size() == stmtExtracted.size());
	for (int i = 0; i < stmtExpected.size(); i++) {
		REQUIRE(stmtExpected[i]->isLineNumberEqual(stmtExtracted[i]));
		REQUIRE(stmtExpected[i]->isStatementTypeEqual(stmtExtracted[i]));
	}
}
