#include "SP/DesignExtractor.h"
#include "SP/SPUtils.h"
#include "catch.hpp"

#include "SP/Parser.h"
#include "SP/Tokenizer.h"

TEST_CASE("extract procedure small program") {
    std::vector<Procedure *> expected;
    expected.push_back(new Procedure("Bedok"));

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ProcedureExtractor extr(program, nullptr);
    std::vector<Procedure *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getName() == extracted[i]->getName());
    }
}

TEST_CASE("extract assignments small program") {
    std::vector<Assignment *> expected;
    Assignment a1 = Assignment(1, "west", "((9)+(east))");
    Assignment a2 = Assignment(2, std::string("y"), "((east)-(4))");
    Assignment a3 = Assignment(3, "z", "((west)+(2))");
    Assignment a4 = Assignment(4, "west", "(((9)+(east))+(west))");
    expected.push_back(&a1);
    expected.push_back(&a2);
    expected.push_back(&a3);
    expected.push_back(&a4);

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    StatementExtractor extr(program, nullptr);
    std::vector<Assignment *> extracted = extr.extractAssignments();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLineNo() == extracted[i]->getLineNo());
        REQUIRE(expected[i]->getExpression() == extracted[i]->getExpression());
        REQUIRE(expected[i]->getVariable() == extracted[i]->getVariable());
    }
}

TEST_CASE("extract statement small program") {
    std::vector<Statement *> expected;
    expected.push_back(new Statement(1, StatementType::ASSIGN));
    expected.push_back(new Statement(2, StatementType::ASSIGN));
    expected.push_back(new Statement(3, StatementType::ASSIGN));
    expected.push_back(new Statement(4, StatementType::ASSIGN));

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    StatementExtractor extr(program, nullptr);
    std::vector<Statement *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->isLineNumberEqual(extracted[i]));
        REQUIRE(expected[i]->isStatementTypeEqual(extracted[i]));
    }
}

TEST_CASE("extract follows small program") {
    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS, 1, 2));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS, 2, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS, 3, 4));

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    FollowsExtractor extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract followsT small program") {
    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 1, 2));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 1, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 2, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 1, 4));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 2, 4));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::FOLLOWS_T, 3, 4));

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    FollowsExtrT extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract parent small program, a IF followed by a WHILE") {
    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 1, 2));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 1, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 1, 4));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 1, 5));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 6, 7));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT, 6, 8));

    std::string sourceProgram =
        "procedure Bedok {\nif (a != 1) then{\na = 2;\na = 3;\n}\nelse {\na = "
        "4;\na = 5;\n }\nwhile (a != 6) {\na = 7;\na = 8;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ParentExtractor extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract parentT small program, WHILE in a IF") {
    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 2));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 3, 4));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 3, 5));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 4));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 5));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 6));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 7));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::PARENT_T, 1, 8));

    std::string sourceProgram =
        "procedure Bedok {\nif (a != 1) then{\na = 2;\nwhile (a != 6) {\na = "
        "4;\na = 5;\n}\na = 6;\n}else {\na = 7;\na = 8;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ParentExtrT extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract usesS small program, ASSIGN and IF and WHILE and PRINT") {
    std::vector<Relationship<int, std::string> *> expected;
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 1, "a"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "b"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 3, "c"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "c"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 4, "d"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "d"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 5, "e"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 6, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 5, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 7, "g"));

    std::string sourceProgram =
        "procedure Bedok {\nx = a;\nif (b != 2) then{\nx = c;\n}\nelse {\nx = "
        "d;\n}\nwhile (e != 5) {\nx = f;\n}\nprint g;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    UsesSExtractor extr(program, nullptr);
    std::vector<Relationship<int, std::string> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract modifiesS small program, ASSIGN and IF and WHILE and READ") {
    std::vector<Relationship<int, std::string> *> expected;
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 1, "a"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 3, "c"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 2, "c"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 4, "d"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 2, "d"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 6, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 5, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::MODIFIES, 7, "g"));

    std::string sourceProgram =
        "procedure Bedok {\na = 1;\nif (b != 2) then{\nc = 3;\n}\nelse {\nd = "
        "4;\n}\nwhile (e != 5) {\nf = 6;\n}\nread g;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ModSExtractor extr(program, nullptr);
    std::vector<Relationship<int, std::string> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Extract program with if-else statements") {
    // Follow relationship
    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship(RelationshipReference::FOLLOWS_T, 3, 4));
    expected.push_back(
        new Relationship(RelationshipReference::FOLLOWS_T, 1, 2));

    std::string sourceProgram = "procedure Bedok{print c; if(a*b!=4) then "
                                "{a=3; while(a*b!=3){a=2;}} else{read c;}}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramParser parser = ProgramParser(0, tokens);
    ProgramNode *program = parser.parse();
    FollowsExtrT extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }

    // Variable extr
    std::vector<std::string> varexpected = {"a", "b", "c"};
    std::vector<Variable *> varresult =
        VariableExtractor(program, nullptr).extract();

    REQUIRE(varexpected.size() == varresult.size());
    for (int i = 0; i < varresult.size(); i++) {
        REQUIRE(find(begin(varexpected), end(varexpected),
                     varresult.at(i)->getName()) != end(varexpected));
    }

    // Constant extr
    std::vector<std::string> constexpected = {"2", "3", "4"};
    ConstantExtractor extractor(program, nullptr);
    std::vector<Constant *> constresult = extractor.extract();

    REQUIRE(constexpected.size() == constresult.size());

    for (int i = 0; i < constresult.size(); i++) {
        REQUIRE(find(begin(constexpected), end(constexpected),
                     constresult.at(i)->getName()) != end(constexpected));
    }

    // Statement exty
    std::vector<Statement *> stmtExpected;
    stmtExpected.push_back(new Statement(1, StatementType::PRINT));
    stmtExpected.push_back(new Statement(2, StatementType::IF));
    stmtExpected.push_back(new Statement(3, StatementType::ASSIGN));
    stmtExpected.push_back(new Statement(4, StatementType::WHILE));
    stmtExpected.push_back(new Statement(5, StatementType::ASSIGN));
    stmtExpected.push_back(new Statement(6, StatementType::READ));

    std::vector<Statement *> stmtExtracted =
        StatementExtractor(program, nullptr).extract();
    REQUIRE(stmtExpected.size() == stmtExtracted.size());
    for (int i = 0; i < stmtExpected.size(); i++) {
        REQUIRE(stmtExpected[i]->isLineNumberEqual(stmtExtracted[i]));
        REQUIRE(stmtExpected[i]->isStatementTypeEqual(stmtExtracted[i]));
    }
}
