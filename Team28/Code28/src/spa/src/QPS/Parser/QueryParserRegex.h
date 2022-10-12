#pragma once

#include "../../commons/Entity.h"
#include "../../commons/Relationship.h"
#include <regex>
#include <string>
#include <unordered_set>
#include <unordered_map>

typedef std::unordered_map<std::string, EntityName> ENTITY_MAP;
typedef std::unordered_map<std::string, EntityAttribute> ENTITY_ATTR_MAP;
typedef std::unordered_map<std::string, RelationshipReference> RELATIONSHIP_MAP;
typedef std::unordered_map<RelationshipReference,
                           std::unordered_set<EntityName>>
    RELATIONSHIP_ARG_MAP;
typedef std::unordered_map<RelationshipReference,
                           std::unordered_set<ReferenceType>>
    RELATIONSHIP_REF_MAP;

/*
 * Lookup tables to map unprocessed strings to their respective enum values
 */
// map string to design entity enum
const ENTITY_MAP entityMap = {
    {"stmt", EntityName::STMT},         {"read", EntityName::READ},
    {"print", EntityName::PRINT},       {"call", EntityName::CALL},
    {"while", EntityName::WHILE},       {"if", EntityName::IF},
    {"assign", EntityName::ASSIGN},     {"variable", EntityName::VARIABLE},
    {"constant", EntityName::CONSTANT}, {"procedure", EntityName::PROCEDURE}
};

// map string to entity attribute enum
const ENTITY_ATTR_MAP entityAttrMap = {
    {"procName", EntityAttribute::PROC_NAME},
    {"varName", EntityAttribute::VAR_NAME},
    {"value", EntityAttribute::VALUE},
    {"stmt#", EntityAttribute::STMT_NO}
};

// map string to relationship enum
const RELATIONSHIP_MAP relationshipMap = {
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
    {"Affects*", RelationshipReference::AFFECTS_T}
};

// map relationship type to valid left arguments
const RELATIONSHIP_ARG_MAP relationshipLeftArgMap = {
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
const RELATIONSHIP_ARG_MAP relationshipRightArgMap = {
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
const RELATIONSHIP_REF_MAP relationshipLeftRefMap = {
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
const RELATIONSHIP_REF_MAP relationshipRightRefMap = {
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
const std::regex intRegex(
    "\\s*0|[1-9]\\d*\\s*"); // integer: DIGIT+

const std::regex synRegex(
    "\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"); // synonym: LETTER(LETTER|DIGIT)*

const std::regex nameRegex(
    "\\s*[a-zA-Z][a-zA-Z0-9]*\\s*");         // name: LETTER ( LETTER | DIGIT )*

const std::regex wildcardRegex("\\s*_\\s*"); // wildcard: _

const std::regex attrRefRegex(
    "\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*"            // synonym
    "[\\.]"                                 // '.'
    "\\s*(procName|varName|value|stmt#)\\s*"    // attrName
);

/*
 * Regex expressions for arguments
 */
// stmtRef : synonym | _ | INTEGER
const std::regex stmtRefRegex(
    "^\\s*([a-zA-Z][a-zA-Z0-9]*" // synonym
    "|_|"                        // | _ |
    "0|[1-9]\\d*?)\\s*$");       // INTEGER

// entRef : synonym | _ | '"' IDENT '"'
const std::regex entRefRegex(
    "^\\s*([a-zA-Z][a-zA-Z0-9]*"                // synonym
    "|_|"                                       // | _ |
    "\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*$"); // '"' IDENT '"'

/*
 * Regex expressions for query clauses
 */
// declaration: design-entity synonym (',' synonym)*
const std::regex declarationRegex(
    "^\\s*(stmt|read|print|call|while|if|assign|"
    "variable|constant|procedure)\\s+"    // design-entity
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"          // synonym
    "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*$"); // (',' synonym)*

// select-cl : declaration* 'Select' synonym [ suchthat-cl ]   [ pattern-cl ]
// For arguments extraction
const std::regex selectRegex(
    "\\s*Select\\s+"                 // 'Select'
    "([a-zA-Z][a-zA-Z0-9]*|BOOLEAN|" // synonym or BOOLEAN
    "<\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"  // or <synonym (, synonym)*>
    "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*>)\\s*");

// For clause extraction
const std::regex selectClauseRegex(
    "(\\s*Select\\s+"                // 'Select'
    "([a-zA-Z][a-zA-Z0-9]*|BOOLEAN|" // synonym or BOOLEAN
    "<\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"  // or <synonym (, synonym)*>
    "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*>)\\s*).*?");

// For select tuple
const std::regex selectTupleRegex(
    "<\\s*[a-zA-Z][a-zA-Z0-9]*\\s*"
    "(,\\s*[a-zA-Z][a-zA-Z0-9]*\\s*)*>");

// suchthat-cl : 'such' 'that' relRef
// relRef : ModifiesP | ModifiesS | UsesP | UsesS
//          | Parent | ParentT | Follows | FollowsT
// For arguments extraction
const std::regex suchThatRegex(
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
const std::regex suchThatClauseRegex(
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
const std::regex patternRegex(
    "(^\\s*pattern\\s+|^\\s*and\\s+)" // pattern or and
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"      // syn-assign or syn-while or syn-if
    "\\(\\s*"                         // '('
    "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))" // entRef
    "\\s*,\\s*"
    "(\\s*\".*?\"\\s*|\\s*_\\s*|\\s*_\\s*\".*?\"\\s*_\\s*)" // expression-spec
    "(\\s*,\\s*_)*"
    "\\s*\\)\\s*");

// For clause extraction
const std::regex patternClauseRegex(
    "(^\\s*(pattern\\s+|^\\s*and\\s+)" // pattern or and
    "([a-zA-Z][a-zA-Z0-9]*)\\s*"       // syn-assign or syn-while or syn-if
    "\\(\\s*"                          // '('
    "(([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"))" // entRef
    "\\s*,\\s*"
    "(\\s*\".*?\"\\s*|\\s*_\\s*|\\s*_\\s*\".*?\"\\s*_\\s*)" // expression-spec
    "(\\s*,\\s*_)*"
    "\\s*\\)).*?");

const std::unordered_set<EntityName> patternEntityMap = {
    EntityName::ASSIGN, EntityName::IF, EntityName::WHILE};

// with-cl : 'with' attrCond
// attrCond : attrCompare ( 'and' attrCompare )*
// attrCompare : ref '=' ref
// ref : '"' IDENT '"'" | INTEGER | attrRef | synonym
// attrRef : synonym '.' attrName
// For arguments extraction
const std::regex withRegex(
    "^\\s*(with\\s+|^\\s*and\\s+)"  // with or and 
    "(.*?)"                         // ref
    "\\s*=\\s*"                     // '='
    "(.*?)\\s*"                     // ref
);

// For clause extraction
const std::regex withClauseRegex(
    "(^\\s*(with\\s+|^\\s*and\\s+)" // with or and 
    "(.*?)"                         // ref
    "\\s*=\\s*"                     // '='
    "(.*?))\\s*"                     // ref
);
