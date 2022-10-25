#pragma once

#include "QueryClause.h"

// map relationship type to valid left arguments
const std::unordered_map<RelationshipReference, std::unordered_set<EntityName>>
    RELATIONSHIP_LEFT_ARG_MAP = {
        {RelationshipReference::FOLLOWS,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::FOLLOWS_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::PARENT,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::PARENT_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::USES,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN, EntityName::PROCEDURE}},
        {RelationshipReference::MODIFIES,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN, EntityName::PROCEDURE}},
        {RelationshipReference::CALLS, {EntityName::PROCEDURE}},
        {RelationshipReference::CALLS_T, {EntityName::PROCEDURE}},
        {RelationshipReference::NEXT,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::NEXT_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::AFFECTS,
         {EntityName::ASSIGN, EntityName::STMT}},
        {RelationshipReference::AFFECTS_T,
         {EntityName::ASSIGN, EntityName::STMT}}};

// map relationship type to valid right arguments
const std::unordered_map<RelationshipReference, std::unordered_set<EntityName>>
    RELATIONSHIP_RIGHT_ARG_MAP = {
        {RelationshipReference::FOLLOWS,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::FOLLOWS_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::PARENT,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::PARENT_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::USES, {EntityName::VARIABLE}},
        {RelationshipReference::MODIFIES, {EntityName::VARIABLE}},
        {RelationshipReference::CALLS, {EntityName::PROCEDURE}},
        {RelationshipReference::CALLS_T, {EntityName::PROCEDURE}},
        {RelationshipReference::NEXT,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::NEXT_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::AFFECTS,
         {EntityName::ASSIGN, EntityName::STMT}},
        {RelationshipReference::AFFECTS_T,
         {EntityName::ASSIGN, EntityName::STMT}}};

// map relationship type to valid left reference
const std::unordered_map<RelationshipReference,
                         std::unordered_set<ReferenceType>>
    RELATIONSHIP_LEFT_REF_MAP = {
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
const std::unordered_map<RelationshipReference,
                         std::unordered_set<ReferenceType>>
    RELATIONSHIP_RIGHT_REF_MAP = {
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

const std::unordered_set<RelationshipReference> noSameSynonym = {
    RelationshipReference::FOLLOWS, RelationshipReference::FOLLOWS_T,
    RelationshipReference::PARENT,  RelationshipReference::PARENT_T,
    RelationshipReference::USES,    RelationshipReference::MODIFIES,
    RelationshipReference::CALLS,   RelationshipReference::CALLS_T,
    RelationshipReference::NEXT};

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause : public QueryClause {
public:
    SuchThatClause() : relationship(RelationshipReference::EMPTY){};
    RelationshipReference getRelationship();
    Reference getRefLeft();
    Reference getRefRight();
    void parse(std::smatch matches, std::vector<Synonym> syns);
    bool validate();
    ClauseResult evaluate(QueryFacade *queryFacade);

private:
    RelationshipReference relationship;
    Reference refLeft;
    Reference refRight;
    ClauseResult handleNoSynonym(QueryFacade *queryFacade);
    ClauseResult handleLeftSynonym(QueryFacade *queryFacade);
    ClauseResult handleRightSynonym(QueryFacade *queryFacade);
    ClauseResult handleBothSynonym(QueryFacade *queryFacade);
};
