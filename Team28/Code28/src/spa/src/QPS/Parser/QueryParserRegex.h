#pragma once

#include <unordered_map>
#include <string>
#include <regex>
#include "../../commons/Entity.h"
#include "../../commons/Relationship.h"


typedef std::unordered_map<std::string, EntityName> ENTITY_LOOKUP;
typedef std::unordered_map<std::string, Relationship> RELATIONSHIP_LOOKUP;

/*
* Lookup tables to map unprocessed strings to their respective enum values
*/
// map string to design entity enum
const ENTITY_LOOKUP entityNameLookup = {
        {"stmt", EntityName::STMT},
        {"read", EntityName::READ},
        {"print", EntityName::PRINT},
        {"call", EntityName::CALL},
        {"while", EntityName::WHILE},
        {"if", EntityName::IF},
        {"assign", EntityName::ASSIGN},
        {"variable", EntityName::VARIABLE},
        {"constant", EntityName::CONSTANT},
        {"procedure", EntityName::PROCEDURE}
};

// map string to relationship enum
const RELATIONSHIP_LOOKUP relationshipNameLookup = {
        {"Follows", Relationship::FOLLOWS},
        {"Follows*", Relationship::FOLLOWS_STAR},
        {"Parent", Relationship::PARENT},
        {"Parent*", Relationship::PARENT_STAR},
        {"Uses", Relationship::USES},
        {"Modifies", Relationship::MODIFIES},
        {"Calls", Relationship::CALLS},
        {"Calls*", Relationship::CALLS_STAR}
};

/*
* Regex expressions for primitive types
*/
const std::regex intRegex("^\\s*0|[1-9]\\d*\\s*$");  // integer: DIGIT+
const std::regex synRegex("^\\s*[a-zA-Z][a-zA-Z0-9]*\\s*$");  // synonym: LETTER(LETTER|DIGIT)*
const std::regex nameRegex("^\\s*\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"\\s*$"); // name: LETTER ( LETTER | DIGIT )*
const std::regex wildcardRegex("^\\s*_\\s*$"); // wildcard: _

/*
* Regex expressions for arguments
*/
// stmtRef : synonym | _ | INTEGER
const std::regex stmtRefRegex(
        "^\\s*([a-zA-Z][a-zA-Z0-9]*"  // synonym
        "|_|"  // | _ |
        "0|[1-9]\\d*?)\\s*$");  // INTEGER

// entRef : synonym | _ | '"' IDENT '"'
const std::regex entRefRegex(
        "^\\s*([a-zA-Z][a-zA-Z0-9]*"  // synonym
        "|_|"  // | _ |
        "\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*$");  // '"' IDENT '"'

/*
* Regex expressions for query clauses
*/
// declaration: design-entity synonym (',' synonym)*
const std::regex declarationRegex(
        "^\\s*(stmt|read|print|call|while|if|assign|"
        "variable|constant|procedure)\\s+"  // design-entity
        "([a-zA-Z][a-zA-Z0-9]*)\\s*"  // synonym
        "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*$");  // (',' synonym)*

// select-cl : declaration* 'Select' synonym [ suchthat-cl ]   [ pattern-cl ]
// For arguments extraction
const std::regex selectRegex(
        "\\s*Select\\s+"  // 'Select'
        "([a-zA-Z][a-zA-Z0-9]*)\\s*");  // synonym
// For clause extraction
const std::regex selectClauseRegex(
        "(\\s*Select\\s+"  // 'Select'
        "([a-zA-Z][a-zA-Z0-9]*)\\s*).*?");  // synonym

// suchthat-cl : 'such' 'that' relRef
// relRef : ModifiesP | ModifiesS | UsesP | UsesS
//          | Parent | ParentT | Follows | FollowsT
// For arguments extraction
const std::regex suchThatRegex(
        "\\s*such\\s+that\\s+"
        "(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|)\\s*" // relRef
        "\\(\\s*"
        "(\\s*[a-zA-z\\d]+\\s*|[a-zA-z\\d]+|_|\\s*0|[1-9]\\d*\\s*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef | stmtRef
        "\\s*,\\s*"
        "(\\s*[a-zA-z\\d]+\\s*|[a-zA-z\\d]+|_|\\s*0|[1-9]\\d*\\s*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef | stmtRef
        "\\s*\\)\\s*");
// For clause extraction
const std::regex suchThatClauseRegex(
        "(\\s*such\\s+that\\s+"
        "(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|)\\s*" // relRef
        "\\(\\s*"
        "(\\s*[a-zA-z\\d]+\\s*|[a-zA-z\\d]+|_|\\s*0|[1-9]\\d*\\s*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef | stmtRef
        "\\s*,\\s*"
        "(\\s*[a-zA-z\\d]+\\s*|[a-zA-z\\d]+|_|\\s*0|[1-9]\\d*\\s*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef | stmtRef
        "\\s*\\)\\s*).*?");

//  pattern-cl : 'pattern' syn-assign '(' entRef ',' expression-spec ')'
//  expression-spec: '"' expr'"' | '_' '"' expr '"' '_' | '_'
//  expr: expr '+' term | expr '-' term | term
//  term: term '*' factor | term '/' factor | factor
//  factor: var_name | const_value | '(' expr ')'
// For arguments extraction
const std::regex patternRegex(
        "\\s*pattern\\s+"  // pattern
        "([a-zA-Z][a-zA-Z0-9]*)\\s*"  // syn-assign
        "\\(\\s*"  // '('
        "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))"  // entRef
        "\\s*,\\s*"
        "(\".*?\"|_|_\".*?\"_)" // expression-spec
        "\\s*\\)\\s*");
// For clause extraction
const std::regex patternClauseRegex(
        "(\\s*pattern\\s+"  // pattern
        "([a-zA-Z][a-zA-Z0-9]*)\\s*"  // syn-assign
        "\\(\\s*"  // '('
        "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))"  // entRef
        "\\s*,\\s*"
        "(\".*?\"|_|_\".*?\"_)" // expression-spec
        "\\s*\\)\\s*$).*?");
