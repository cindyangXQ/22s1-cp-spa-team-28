#include "SP/DesignExtractor.h"
#include "SP/SPUtils.h"
#include "catch.hpp"

#include "SP/Parser.h"
#include "SP/Tokenizer.h"

#include <iostream>

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

    // procedure Bedok {
    //    x = a;
    //    if (b != 2) then {
    //        x = c;
    //    }
    //    else {
    //        x = d;
    //    }
    //    while (e != 5) {
    //        x = f;
    //    }
    //    print g;
    //}
    std::string sourceProgram =
        "procedure Bedok {\nx = a;\nif (b != 2)\nthen { x = c; }\nelse {\nx = "
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

TEST_CASE("extract usesS, nested if/while") {
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
        RelationshipReference::USES, 5, "e"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 4, "e"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 6, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 4, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "d"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "e"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "f"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 7, "g"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "g"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 8, "h"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 9, "i"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 8, "i"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 10, "j"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 8, "j"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "h"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "i"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 2, "j"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 11, "k"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 12, "l"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 11, "l"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 13, "m"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 14, "n"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 13, "n"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 11, "m"));
    expected.push_back(new Relationship<int, std::string>(
        RelationshipReference::USES, 11, "n"));

    // procedure Bedok {
    //     x = a;
    //
    //     if (b != 2) then {
    //         x = c;
    //         if (d == 4) then {
    //             x = e;
    //         } else {
    //             x = f;
    //         }
    //     }
    //     else {
    //         x = g;
    //         if (h == 8) then {
    //             x = i;
    //         } else {
    //             x = j;
    //         }
    //     }
    //
    //     while (k != 11) {
    //         x = l;
    //         while (m == 13) {
    //             x = n;
    //         }
    //     }
    // }
    std::string sourceProgram =
        "procedure Bedok {\nx = a;\nif (b != 2) then {\nx = c;\nif (d == 4) "
        "then {\nx = e;\n} else {\nx = f;\n}\n}\nelse {\nx = g;\nif (h == 8) "
        "then {\nx = i;\n} else {\nx = j;\n}\n}\n\nwhile (k != 11) {\nx = "
        "l;\nwhile (m == 13) {\nx = n;\n}\n}\n}";
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

TEST_CASE("extract usesP small program") {
    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "d"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "f"));

    // procedure Bedok {
    //    read a;
    //    print b;
    //    x = c;
    //    if (d != 2) then {
    //        x = e;
    //    } else {
    //        x = f;
    //    }
    //}
    std::string sourceProgram =
        "procedure Bedok {\nread a;\nprint b;\nx = c;\nif (d != 2) then { \nx "
        "= e;\n} else {\nx = f;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    UsesPExtractor extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract usesP, one call") {
    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "d"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "h"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "i"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "j"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Bedok", "k"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Queenstown", "h"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Queenstown", "i"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Queenstown", "j"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::USES, "Queenstown", "k"));

    // procedure Bedok {
    //    read a;
    //    print b;
    //    x = c;
    //    if (d != 2) then {
    //        x = e;
    //    } else {
    //        x = f;
    //    }
    //    call Queenstown;
    //}
    // procedure Queenstown {
    //    read g;
    //    print h;
    //    x = i;
    //    while (j != 2) {
    //        x = k;
    //    }
    //}
    std::string sourceProgram =
        "procedure Bedok {\nread a;\nprint b;\nx = c;\nif (d != 2)\nthen { \nx "
        "= e;\n}\nelse {\nx = f;\n}\ncall Queenstown;\n}\nprocedure Queenstown "
        "{\nread g;\nprint h;\nx = i;\nwhile (j != 2) {\nx = k;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    UsesPExtractor extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

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

    // procedure Bedok {
    //    a = 1;
    //    if(b != 2) then {
    //        c = 3;
    //    }
    //    else {
    //        d = 4;
    //    }
    //    while(e != 5) {
    //        f = 6;
    //    }
    //    read g;
    //}
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

TEST_CASE("extract modifiesP, one procedure") {
    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "a"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "f"));

    // procedure Bedok {
    //     read a;
    //     print b;
    //     c = 3;
    //     if (d != 4) then {
    //         e = 5;
    //     } else {
    //         f = 6;
    //     }
    // }
    std::string sourceProgram =
        "procedure Bedok {\nread a;\nprint b;\nc = 3;\nif (d != 4) then {\ne = "
        "5;\n} else {\nf = 6;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ModPExtractor extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("extract modifiesP, two procedures one call") {
    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "a"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "i"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Bedok", "k"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Queenstown", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Queenstown", "i"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::MODIFIES, "Queenstown", "k"));

    // procedure Bedok {
    //     read a;
    //     print b;
    //     c = 3;
    //     if (d != 4) then {
    //         e = 5;
    //     } else {
    //         f = 6;
    //     }
    //     call Queenstown;
    // }
    // procedure Queenstown {
    //     read g;
    //     print h;
    //     i = 9;
    //     while (j != 10) {
    //         k = 11;
    //     }
    // }
    std::string sourceProgram =
        "procedure Bedok {\nread a;\nprint b;\nc = 3;\nif (d != 4) then {\ne = "
        "5;\n} else {\nf = 6;\n}\ncall Queenstown;\n}\nprocedure Queenstown "
        "{\nread g;\nprint h;\ni = 9;\nwhile (j != 10) {\nk = 11;\n}\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    ModPExtractor extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Test extract Calls") {
    std::string sourceProgram = "procedure a { call b; call c; call f; call g;}"
                                "procedure b { call e; call f; call c;}"
                                "procedure c { call g;}"
                                "procedure d { call a;}"
                                "procedure e { call c;}"
                                "procedure f { read x;}"
                                "procedure g { print y;}";

    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "a", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "a", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "a", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "a", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "b", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "b", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "b", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "c", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "d", "a"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS, "e", "c"));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    CallsExtractor extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Test extract CallsT") {
    std::string sourceProgram = "procedure a { call b; call c; call f; call g;}"
                                "procedure b { call e; call f; call c;}"
                                "procedure c { call g;}"
                                "procedure d { call a;}"
                                "procedure e { call c;}"
                                "procedure f { read x;}"
                                "procedure g { print y;}";

    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "c", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "a"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "e", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "e", "g"));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    CallsExtrT extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Test extract Calls/* in program with container") {
    std::string sourceProgram =
        "procedure a { if(x == 9) then {call b;} else { if (y == 2) then {call "
        "c;} else {call f; while(t>0){call g;}}}}"
        "procedure b { if(x == y) then {call e;} else { while(y<=9){call f;}} "
        "call c;}"
        "procedure c { call g;}"
        "procedure d { call a;}"
        "procedure e { call c;}"
        "procedure f { read x;}"
        "procedure g { print y;}";
    std::vector<Relationship<std::string, std::string> *> expected;
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "a", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "b", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "c", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "a"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "b"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "f"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "g"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "d", "e"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "e", "c"));
    expected.push_back(new Relationship<std::string, std::string>(
        RelationshipReference::CALLS_T, "e", "g"));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    CallsExtrT extr(program, nullptr);
    std::vector<Relationship<std::string, std::string> *> extracted =
        extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Test BranchIn/Out extractor in simple program") {
    std::string sourceProgram = " procedure a {"
                                "    if(x == y) then { "
                                "        print x; x = 2 * y + 9;}"
                                "    else {"
                                "        read y; y = 2 * x + 1;}"
                                "    while ( x < 3) { x = 2; }}";

    // Branch In
    std::vector<Relationship<int, int> *> inexpected;

    inexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 1, 2));
    inexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 1, 4));
    inexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 6, 7));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramParser parser = ProgramParser(0, tokens);
    ProgramNode *program = parser.parse();
    BranchInExtr extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(inexpected.size() == extracted.size());
    for (int i = 0; i < inexpected.size(); i++) {
        REQUIRE(inexpected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(inexpected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(inexpected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }

    // Branch Out
    std::vector<Relationship<int, int> *> outexpected;
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 3, 6));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 5, 6));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 7, 6));

    BranchOutExtr outextr(program, nullptr);
    extracted = outextr.extract();

    REQUIRE(outexpected.size() == extracted.size());
    for (int i = 0; i < inexpected.size(); i++) {
        REQUIRE(outexpected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(outexpected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(outexpected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Test BranchIn extractor in program with nested if-else statement") {
    std::string sourceProgram = "procedure a {"
                                "    if (x == y) then {"
                                "       if(x < y) then {"
                                "          read x;"
                                "          print y;"
                                "          while ( y == 3 ) {"
                                "            x = x+1;"
                                "            while ( x == 4) {"
                                "                y = y - 8;"
                                "            }"
                                "          }"
                                "        } else { a = a+ 1; }"
                                "        read y;"
                                "    } else {"
                                "        read = (7*a) + 5;"
                                "        while(a > 9) {"
                                "            if (x < y) then {"
                                "                read p;"
                                "                print q;"
                                "            } else {"
                                "                t = t + 1;"
                                "            }"
                                "        }"
                                "    }"
                                "    print b;"
                                "}";

    // Branch In
    std::vector<Relationship<int, int> *> expected;

    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 1, 2));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 1, 11));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 2, 3));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 2, 9));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 5, 6));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 7, 8));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 12, 13));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 13, 14));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 13, 16));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramParser parser = ProgramParser(0, tokens);
    ProgramNode *program = parser.parse();
    BranchInExtr extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

    REQUIRE(expected.size() == extracted.size());
    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(expected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(expected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }

    // BranchOut
    std::vector<Relationship<int, int> *> outexpected;

    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 10, 17));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 12, 17));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 5, 10));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 9, 10));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 7, 5));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 8, 7));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 15, 12));
    outexpected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 16, 12));

    BranchOutExtr outextr(program, nullptr);
    extracted = outextr.extract();

    REQUIRE(outexpected.size() == extracted.size());
    for (int i = 0; i < outexpected.size(); i++) {
        REQUIRE(outexpected[i]->getLeft() == extracted[i]->getLeft());
        REQUIRE(outexpected[i]->getRight() == extracted[i]->getRight());
        REQUIRE(outexpected[i]->getRelationshipReference() ==
                extracted[i]->getRelationshipReference());
    }
}

TEST_CASE("Extract Branch out from program with if nested in while") {
    std::string sourceProgram = "procedure a {"
                                "    while(x == y) {"
                                "        read x;"
                                "        read y;"
                                "        if ( x == y ) then {"
                                "            if( x < 10) then {"
                                "                print x;"
                                "                print y;"
                                "                while ( x == y) {"
                                "                    x = x+1;"
                                "                }"
                                "            }  else {"
                                "                y = y + 1;"
                                "            }"
                                "        } else {"
                                "            p = p + 1;"
                                "        }"
                                "    }"
                                "}";

    std::vector<Relationship<int, int> *> expected;
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 11, 1));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 8, 1));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 10, 1));
    expected.push_back(
        new Relationship<int, int>(RelationshipReference::NEXT, 9, 8));

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramParser parser = ProgramParser(0, tokens);
    ProgramNode *program = parser.parse();
    BranchOutExtr extr(program, nullptr);
    std::vector<Relationship<int, int> *> extracted = extr.extract();

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
