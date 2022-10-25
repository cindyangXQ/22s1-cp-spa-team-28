#pragma once

#include "TableValue.h"

/*
 * Enumerates the different design entity.
 */
enum class RelationshipReference {
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    USES,
    MODIFIES,
    CALLS,
    CALLS_T,
    NEXT,
    NEXT_T,
    AFFECTS,
    AFFECTS_T,
    EMPTY
};

// map string to relationship enum
const std::unordered_map<std::string, RelationshipReference> RELATIONSHIP_MAP =
    {{"Follows", RelationshipReference::FOLLOWS},
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

/*
 * A class encapsulating Relationships in SIMPLE.
 */
template <typename Left, typename Right>
class Relationship : public TableValue {
public:
    /*
     * Explicit constructor for Relationship.
     */
    explicit Relationship(RelationshipReference relRef, Left left,
                          Right right) {
        this->relRef = relRef;
        this->left = left;
        this->right = right;
    };

    Left getLeft() { return this->left; };

    Right getRight() { return this->right; };

    RelationshipReference getRelationshipReference() { return this->relRef; }

private:
    RelationshipReference relRef;
    Left left;
    Right right;
};
