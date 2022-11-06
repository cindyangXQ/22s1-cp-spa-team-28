#pragma once

#include "../../commons/Entity.h"
#include "../../commons/Relationship.h"
#include "../Clause/PatternClause.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include "../Clause/WithClause.h"
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

/*
 * Lookup tables to map unprocessed strings to their respective enum values
 */
// map string to design entity enum
const std::unordered_map<std::string, EntityName> ENTITY_MAP = {
    {"stmt", EntityName::STMT},         {"read", EntityName::READ},
    {"print", EntityName::PRINT},       {"call", EntityName::CALL},
    {"while", EntityName::WHILE},       {"if", EntityName::IF},
    {"assign", EntityName::ASSIGN},     {"variable", EntityName::VARIABLE},
    {"constant", EntityName::CONSTANT}, {"procedure", EntityName::PROCEDURE}};

// map string to entity attribute enum
const std::unordered_map<std::string, EntityAttribute> ENTITY_ATTR_MAP = {
    {"procName", EntityAttribute::PROC_NAME},
    {"varName", EntityAttribute::VAR_NAME},
    {"value", EntityAttribute::VALUE},
    {"stmt#", EntityAttribute::STMT_NO}};

/*
 * Regex expressions for primitive types
 */
const std::regex INT_REGEX("\\s*0|[1-9]\\d*\\s*"); // integer: DIGIT+

const std::regex
    SYN_REGEX("\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"); // synonym: LETTER(LETTER|DIGIT)*

const std::regex NAME_REGEX(
    "\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"); // name: LETTER ( LETTER | DIGIT )*

const std::regex WILDCARD_REGEX("\\s*_\\s*"); // wildcard: _

const std::regex
    ATTR_REF_REGEX("\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*"         // synonym
                   "\\."                                    // '.'
                   "\\s*(procName|varName|value|stmt#)\\s*" // attrName
    );

/*
 * Regex expressions for arguments
 */
// stmtRef : synonym | _ | INTEGER
const std::regex STMT_REF_REGEX("^\\s*([a-zA-Z][a-zA-Z0-9]*" // synonym
                                "|_|"                        // | _ |
                                "0|[1-9]\\d*?)\\s*$");       // INTEGER

// entRef : synonym | _ | '"' IDENT '"'
const std::regex
    ENT_REF_REGEX("^\\s*([a-zA-Z][a-zA-Z0-9]*"                // synonym
                  "|_|"                                       // | _ |
                  "\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*$"); // '"' IDENT '"'

const std::regex IS_SELECT("\\b(Select)\\b");
const std::regex IS_SUCH_THAT("^\\s*such\\s+that\\s+");
const std::regex IS_PATTERN("^\\s*pattern\\s+");
const std::regex IS_WITH("^\\s*with\\s+");
const std::regex IS_SUCH_THAT_AND("^\\s*such\\s+that\\s+|^\\s*and\\s+");
const std::regex IS_PATTERN_AND("^\\s*pattern\\s+|^\\s*and\\s+");
const std::regex IS_WITH_AND("^\\s*with\\s+|^\\s*and\\s+");

/*
 * Regex expressions for query clauses
 */
// declaration: design-entity synonym (',' synonym)*
const std::regex
    DECL_REGEX("^\\s*(stmt|read|print|call|while|if|assign|"
               "variable|constant|procedure)\\s+"    // design-entity
               "([a-zA-Z][a-zA-Z0-9]*)\\s*"          // synonym
               "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*$"); // (',' synonym)*

// select-cl : declaration* 'Select' synonym [ suchthat-cl ]   [ pattern-cl ]
// For arguments extraction
const std::regex
    SELECT_REGEX("\\s*Select\\s+" // 'Select'
                 "([a-zA-Z][a-zA-Z0-9]*"
                 "(\\s*[\\.]\\s*(procName|varName|value|stmt#))*|"
                 "BOOLEAN|" // synonym or BOOLEAN or attr
                 "<.*?"     // or <synonym or attr (, synonym or attr)*>
                 "(,.*?)*>)\\s*");

// For clause extraction
const std::regex
    SELECT_CL_REGEX("(\\s*Select\\s+" // 'Select'
                    "([a-zA-Z][a-zA-Z0-9]*"
                    "(\\s*[\\.]\\s*(procName|varName|value|stmt#))*|"
                    "BOOLEAN|" // synonym or BOOLEAN or attr
                    "<.*?"     // or <synonym (, synonym)*>
                    "(,.*?)*>)\\s*).*?");

// For select tuple
const std::regex SELECT_TUP_REGEX("<.*?"
                                  "(,.*?)*>");

// For select bool
const std::regex SELECT_BOOL_REGEX("\\s*BOOLEAN\\s*");

// suchthat-cl : 'such' 'that' relRef
// relRef : ModifiesP | ModifiesS | UsesP | UsesS
//          | Parent | ParentT | Follows | FollowsT
// For arguments extraction
const std::regex SUCH_THAT_REGEX(
    "(^\\s*such\\s+that\\s+|^\\s*and\\s+)"
    "(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|Calls|Calls\\*|Next|"
    "Next\\*|Affects|Affects\\*)\\s*" // relRef
    "\\(\\s*"
    "([a-zA-z\\d]+|_|0|[1-9]\\d*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef |
                                                                    // stmtRef
    "\\s*,\\s*"
    "([a-zA-z\\d]+|_|0|[1-9]\\d*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef |
                                                                    // stmtRef
    "\\s*\\)\\s*");

// For clause extraction
const std::regex SUCH_THAT_CL_REGEX(
    "(^\\s*(such\\s+that\\s+|^\\s*and\\s+)"
    "(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|Calls|Calls\\*|Next|"
    "Next\\*|Affects|Affects\\*)\\s*" // relRef
    "\\(\\s*"
    "([a-zA-z\\d]+|_|0|[1-9]\\d*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef |
                                                                    // stmtRef
    "\\s*,\\s*"
    "([a-zA-z\\d]+|_|0|[1-9]\\d*|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")" // entRef |
                                                                    // stmtRef
    "\\s*\\)).*?");

//  pattern-cl : 'pattern' syn-assign '(' entRef ',' expression-spec ')'
//  expression-spec: '"' expr'"' | '_' '"' expr '"' '_' | '_'
//  expr: expr '+' term | expr '-' term | term
//  term: term '*' factor | term '/' factor | factor
//  factor: var_name | const_value | '(' expr ')'
// For arguments extraction
const std::regex PATTERN_REGEX(
    "(^\\s*pattern\\s+|^\\s*and\\s+)" // pattern or and
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"      // syn-assign or syn-while or syn-if
    "\\(\\s*"                         // '('
    "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))" // entRef
    "\\s*,\\s*"
    "(\\s*\".*?\"\\s*|\\s*_\\s*|\\s*_\\s*\".*?\"\\s*_\\s*)" // expression-spec
    "((\\s*,\\s*_)*)"
    "\\s*\\)\\s*");

// For clause extraction
const std::regex PATTERN_CL_REGEX(
    "(^\\s*(pattern\\s+|^\\s*and\\s+)" // pattern or and
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"       // syn-assign or syn-while or syn-if
    "\\(\\s*"                          // '('
    "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))" // entRef
    "\\s*,\\s*"
    "(\\s*\".*?\"\\s*|\\s*_\\s*|\\s*_\\s*\".*?\"\\s*_\\s*)" // expression-spec
    "((\\s*,\\s*_)*)"
    "\\s*\\)).*?");

// with-cl : 'with' attrCond
// attrCond : attrCompare ( 'and' attrCompare )*
// attrCompare : ref '=' ref
// ref : '"' IDENT '"'" | INTEGER | attrRef | synonym
// attrRef : synonym '.' attrName
// For arguments extraction
const std::regex
    WITH_REGEX("^\\s*(with\\s+|^\\s*and\\s+)" // with or and
               "(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|0|[1-9]\\d*|"
               "[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*procName|[a-zA-Z][a-zA-Z0-9]*"
               "\\s*\\.\\s*varName|[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*value|[a-zA-"
               "Z][a-zA-Z0-9]*\\s*\\.\\s*stmt#)" // ref
               "\\s*=\\s*"                       // =
               "(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|0|[1-9]\\d*|"
               "[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*procName|[a-zA-Z][a-zA-Z0-9]*"
               "\\s*\\.\\s*varName|[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*value|[a-zA-"
               "Z][a-zA-Z0-9]*\\s*\\.\\s*stmt#)\\s*" // ref
    );

// For clause extraction
const std::regex
    WITH_CL_REGEX("(^\\s*(with\\s+|^\\s*and\\s+)" // with or and
                  "(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|0|[1-9]\\d*|"
                  "[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*procName|[a-zA-Z][a-zA-Z0-9]*"
                  "\\s*\\.\\s*varName|[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*value|[a-"
                  "zA-Z][a-zA-Z0-9]*\\s*\\.\\s*stmt#)" // ref
                  "\\s*=\\s*"                          // =
                  "(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|0|[1-9]\\d*|"
                  "[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*procName|[a-zA-Z][a-zA-Z0-9]*"
                  "\\s*\\.\\s*varName|[a-zA-Z][a-zA-Z0-9]*\\s*\\.\\s*value|[a-"
                  "zA-Z][a-zA-Z0-9]*\\s*\\.\\s*stmt#)).*?" // ref
    );

const std::unordered_map<std::type_index, std::regex> IS_CLAUSE_MAP = {
    {typeid(SelectClause), IS_SELECT},
    {typeid(SuchThatClause), IS_SUCH_THAT},
    {typeid(PatternClause), IS_PATTERN},
    {typeid(WithClause), IS_WITH}};

const std::unordered_map<std::type_index, std::regex> IS_CLAUSE_AND_MAP = {
    {typeid(SuchThatClause), IS_SUCH_THAT_AND},
    {typeid(PatternClause), IS_PATTERN_AND},
    {typeid(WithClause), IS_WITH_AND}};

const std::unordered_map<std::type_index, std::regex> WHOLE_CLAUSE_MAP = {
    {typeid(SelectClause), SELECT_CL_REGEX},
    {typeid(SuchThatClause), SUCH_THAT_CL_REGEX},
    {typeid(PatternClause), PATTERN_CL_REGEX},
    {typeid(WithClause), WITH_CL_REGEX}};

const std::unordered_map<std::type_index, std::regex> ARG_CLAUSE_MAP = {
    {typeid(SelectClause), SELECT_REGEX},
    {typeid(SuchThatClause), SUCH_THAT_REGEX},
    {typeid(PatternClause), PATTERN_REGEX},
    {typeid(WithClause), WITH_REGEX}};