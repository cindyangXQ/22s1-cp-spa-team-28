#pragma once

#include "../../commons/Entity.h"
#include "../../commons/Relationship.h"
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_map<std::string, EntityName> EntityMap;
typedef std::unordered_map<std::string, EntityAttribute> EntityAttrMap;
typedef std::unordered_map<std::string, RelationshipReference> RelationshipMap;
typedef std::unordered_map<RelationshipReference,
                           std::unordered_set<EntityName>>
    RelationshipArgMap;
typedef std::unordered_map<RelationshipReference,
                           std::unordered_set<ReferenceType>>
    RelationshipRefMap;

/*
 * Lookup tables to map unprocessed strings to their respective enum values
 */
// map string to design entity enum
const EntityMap ENTITY_MAP = {
    {"stmt", EntityName::STMT},         {"read", EntityName::READ},
    {"print", EntityName::PRINT},       {"call", EntityName::CALL},
    {"while", EntityName::WHILE},       {"if", EntityName::IF},
    {"assign", EntityName::ASSIGN},     {"variable", EntityName::VARIABLE},
    {"constant", EntityName::CONSTANT}, {"procedure", EntityName::PROCEDURE}};

// map string to entity attribute enum
const EntityAttrMap ENTITY_ATTR_MAP = {{"procName", EntityAttribute::PROC_NAME},
                                       {"varName", EntityAttribute::VAR_NAME},
                                       {"value", EntityAttribute::VALUE},
                                       {"stmt#", EntityAttribute::STMT_NO}};

// map string to relationship enum
const RelationshipMap RELATIONSHIP_MAP = {
    {"Follows", RelationshipReference::FOLLOWS},
    {"Follows*", RelationshipReference::FOLLOWS_T},
    {"Parent", RelationshipReference::PARENT},
    {"Parent*", RelationshipReference::PARENT_T},
    {"Uses", RelationshipReference::USES},
    {"Modifies", RelationshipReference::MODIFIES},
    {"Calls", RelationshipReference::CALLS},
    {"Calls*", RelationshipReference::CALLS_T},
    {"Next", RelationshipReference::NEXT},
    {"Next*", RelationshipReference::NEXT_T},
    {"Affects", RelationshipReference::AFFECTS},
    {"Affects*", RelationshipReference::AFFECTS_T}};

// map relationship type to valid left arguments
const RelationshipArgMap RELATIONSHIP_LEFT_ARG_MAP = {
    {RelationshipReference::FOLLOWS,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::FOLLOWS_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::PARENT,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::PARENT_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::USES,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN,
      EntityName::PROCEDURE}},
    {RelationshipReference::MODIFIES,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN,
      EntityName::PROCEDURE}},
    {RelationshipReference::CALLS, {EntityName::PROCEDURE}},
    {RelationshipReference::CALLS_T, {EntityName::PROCEDURE}},
    {RelationshipReference::NEXT,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::NEXT_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::AFFECTS, {EntityName::ASSIGN}},
    {RelationshipReference::AFFECTS_T, {EntityName::ASSIGN}}};

// map relationship type to valid right arguments
const RelationshipArgMap RELATIONSHIP_RIGHT_ARG_MAP = {
    {RelationshipReference::FOLLOWS,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::FOLLOWS_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::PARENT,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::PARENT_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::USES, {EntityName::VARIABLE}},
    {RelationshipReference::MODIFIES, {EntityName::VARIABLE}},
    {RelationshipReference::CALLS, {EntityName::PROCEDURE}},
    {RelationshipReference::CALLS_T, {EntityName::PROCEDURE}},
    {RelationshipReference::NEXT,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::NEXT_T,
     {EntityName::STMT, EntityName::READ, EntityName::PRINT, EntityName::CALL,
      EntityName::WHILE, EntityName::IF, EntityName::ASSIGN}},
    {RelationshipReference::AFFECTS, {EntityName::ASSIGN}},
    {RelationshipReference::AFFECTS_T, {EntityName::ASSIGN}}};

// map relationship type to valid left reference
const RelationshipRefMap RELATIONSHIP_LEFT_REF_MAP = {
    {RelationshipReference::FOLLOWS,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::FOLLOWS_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::PARENT,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::PARENT_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::USES,
     {ReferenceType::STMT_REF, ReferenceType::ENT_REF}},
    {RelationshipReference::MODIFIES,
     {ReferenceType::STMT_REF, ReferenceType::ENT_REF}},
    {RelationshipReference::CALLS,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::CALLS_T,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::NEXT,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::NEXT_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::AFFECTS,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::AFFECTS_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}}};

// map relationship type to valid left reference
const RelationshipRefMap RELATIONSHIP_RIGHT_REF_MAP = {
    {RelationshipReference::FOLLOWS,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::FOLLOWS_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::PARENT,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::PARENT_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::USES,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::MODIFIES,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::CALLS,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::CALLS_T,
     {ReferenceType::ENT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::NEXT,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::NEXT_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::AFFECTS,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}},
    {RelationshipReference::AFFECTS_T,
     {ReferenceType::STMT_REF, ReferenceType::WILDCARD}}};

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
    "Next\\*|Affect|Affect\\*)\\s*" // relRef
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
    "Next\\*|Affect|Affect\\*)\\s*" // relRef
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
    "(\\s*,\\s*_)*"
    "\\s*\\)\\s*");

// For clause extraction
const std::regex PATTERN_CL_REGEX(
    "(^\\s*(pattern\\s+|^\\s*and\\s+)" // pattern or and
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"       // syn-assign or syn-while or syn-if
    "\\(\\s*"                          // '('
    "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))" // entRef
    "\\s*,\\s*"
    "(\\s*\".*?\"\\s*|\\s*_\\s*|\\s*_\\s*\".*?\"\\s*_\\s*)" // expression-spec
    "(\\s*,\\s*_)*"
    "\\s*\\)).*?");

const std::unordered_set<EntityName> PATTERN_ENTITY_MAP = {
    EntityName::ASSIGN, EntityName::IF, EntityName::WHILE};

// with-cl : 'with' attrCond
// attrCond : attrCompare ( 'and' attrCompare )*
// attrCompare : ref '=' ref
// ref : '"' IDENT '"'" | INTEGER | attrRef | synonym
// attrRef : synonym '.' attrName
// For arguments extraction
const std::regex WITH_REGEX("^\\s*(with\\s+|^\\s*and\\s+)" // with or and
                            "(.*?)"                        // ref
                            "\\s*=\\s*"                    // '='
                            "(.*?)\\s*"                    // ref
);

// For clause extraction
const std::regex WITH_CL_REGEX("(^\\s*(with\\s+|^\\s*and\\s+)" // with or and
                               "(.*?)"                         // ref
                               "\\s*=\\s*"                     // '='
                               "(.*?))\\s*"                    // ref
);
