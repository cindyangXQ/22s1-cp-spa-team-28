#include "SP/ProgramParser/EntityNode.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "catch.hpp"

#include <iostream>
#include <vector>

std::string small_expected[] = {
    "procedure", "Bedok", "{", "west", "=", "9",    "+", "east", ";", "y",
    "=",         "east",  "-", "4",    ";", "z",    "=", "west", "+", "2",
    ";",         "west",  "=", "9",    "+", "east", "+", "west", ";"};

TEST_CASE("Tokenize a normal spacing small program") {
    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest= 9 + east + west;\n}";
    Tokenizer tokenizer = Tokenizer(sourceProgram);
    std::vector<Token *> tokens = tokenizer.tokenize();
    for (int i = 0; i < small_expected->size(); i++) {
        REQUIRE(tokens[i]->equals(small_expected[i]));
    }
}

TEST_CASE("Tokenize a less spacing small program") {
    std::string sourceProgram =
        "procedure Bedok "
        "{\nwest=9+east;\ny=east-4;\nz=west+2;\nwest=9+east+west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    for (int i = 0; i < small_expected->size(); i++) {
        REQUIRE(tokens[i]->equals(small_expected[i]));
    }
}

TEST_CASE("Tokenize a least spacing small program") {
    std::string sourceProgram =
        "procedure Bedok{west=9+east;y=east-4;z=west+2;west=9+east+west;}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    for (int i = 0; i < small_expected->size(); i++) {
        REQUIRE(tokens[i]->equals(small_expected[i]));
    }
}

TEST_CASE("Tokenize an extra spacing small program") {
    std::string sourceProgram =
        "procedure   Bedok \t { \n  west\t= \t9 +    east  ;\n   y =   east - "
        "4 ; z = west +   2   \t;  west = 9 + east + west  ; \n}\n";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    for (int i = 0; i < small_expected->size(); i++) {
        REQUIRE(tokens[i]->equals(small_expected[i]));
    }
}

TEST_CASE("Source program with keyword as variable name") {
    std::string sourceProgram = "procedure procedure {"
                                "   call call;"
                                "   procedure = call + print * read;"
                                "   if (if == else) then { read read; }"
                                "   else { print read;}}";

    std::vector<Token *> expected;
    expected.push_back(new Keyword("procedure"));
    expected.push_back(new VariableNode("procedure"));
    expected.push_back(new Symbol("{"));
    expected.push_back(new Keyword("call"));
    expected.push_back(new VariableNode("call"));
    expected.push_back(new Symbol(";"));
    expected.push_back(new VariableNode("procedure"));
    expected.push_back(new Operator("="));
    expected.push_back(new VariableNode("call"));
    expected.push_back(new Operator("+"));
    expected.push_back(new VariableNode("print"));
    expected.push_back(new Operator("*"));
    expected.push_back(new VariableNode("read"));
    expected.push_back(new Symbol(";"));
    expected.push_back(new Keyword("if"));
    expected.push_back(new Symbol("("));
    expected.push_back(new VariableNode("if"));
    expected.push_back(new Operator("=="));
    expected.push_back(new VariableNode("else"));
    expected.push_back(new Symbol(")"));
    expected.push_back(new Keyword("then"));
    expected.push_back(new Symbol("{"));
    expected.push_back(new Keyword("read"));
    expected.push_back(new VariableNode("read"));
    expected.push_back(new Symbol(";"));
    expected.push_back(new Symbol("}"));
    expected.push_back(new Keyword("else"));
    expected.push_back(new Symbol("{"));
    expected.push_back(new Keyword("print"));
    expected.push_back(new VariableNode("read"));
    expected.push_back(new Symbol(";"));
    expected.push_back(new Symbol("}"));
    expected.push_back(new Symbol("}"));

    std::vector<Token *> result = Tokenizer(sourceProgram).tokenize();
    REQUIRE(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        REQUIRE(expected[i]->equals(expected[i]));
    }
}

TEST_CASE("invalid source program") {
    // invalid operator
    std::string sourceProgram = "hello === 1";
    REQUIRE_THROWS(Tokenizer(sourceProgram).tokenize(), "invalid operator");

    // invalid character
    sourceProgram = "~hello;";
    REQUIRE_THROWS(Tokenizer(sourceProgram).tokenize(), "invalid character");

}
