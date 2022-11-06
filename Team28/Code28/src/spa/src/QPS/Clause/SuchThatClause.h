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
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::AFFECTS_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}}};

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
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}},
        {RelationshipReference::AFFECTS_T,
         {EntityName::STMT, EntityName::READ, EntityName::PRINT,
          EntityName::CALL, EntityName::WHILE, EntityName::IF,
          EntityName::ASSIGN}}};

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

const std::unordered_map<RelationshipReference, Designation>
    RELATIONSHIP_DESIGNATION_MAP = {
        {RelationshipReference::FOLLOWS, Designation::FOLLOWS},
        {RelationshipReference::FOLLOWS_T, Designation::FOLLOWS_T},
        {RelationshipReference::PARENT, Designation::PARENT},
        {RelationshipReference::PARENT_T, Designation::PARENT_T},
        {RelationshipReference::MODIFIES, Designation::MOD_S},
        {RelationshipReference::USES, Designation::USE_S},
        {RelationshipReference::CALLS, Designation::CALL},
        {RelationshipReference::CALLS_T, Designation::CALL_T},
        {RelationshipReference::NEXT, Designation::NEXT},
        {RelationshipReference::NEXT_T, Designation::NEXT_T},
        {RelationshipReference::AFFECTS, Designation::AFFECTS},
        {RelationshipReference::AFFECTS_T, Designation::AFFECTS_T}};

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
    std::unordered_set<std::string> getSynonymsUsed();
    void populateOptimizeScore(QueryFacade *queryFacade);
    double getOptimizeScore();
    bool replace(Reference synRef, Reference valRef);

private:
    RelationshipReference relationship;
    Reference refLeft;
    Reference refRight;
    std::unordered_set<std::string> synsUsed;
    double score;
    void populateSynsUsed();
    ClauseResult handleNoSynonym(QueryFacade *queryFacade);
    ClauseResult handleLeftSynonym(QueryFacade *queryFacade);
    ClauseResult handleRightSynonym(QueryFacade *queryFacade);
    ClauseResult handleBothSynonym(QueryFacade *queryFacade);
};
