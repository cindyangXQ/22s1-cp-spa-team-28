#pragma once

#include "Value.h"
#include "Synonym.h"

/*
 * Enumerates the different type of relationship.
 */
enum class RelationshipReference {
    Modifies,
    Uses,
    Parent,
    ParentT,
    Follows,
    FollowsT,
    Empty
};

enum class ReferenceType {
    STMT_REF,
    ENT_REF,
    WILDCARD
};

/*
 * Class encapsulating Reference used in Relationship.
 */
class Reference {
public:
    ReferenceType type;
    bool isSynonym;
    Synonym *syn = nullptr;
    Value value;
    explicit Reference();
    Reference(Synonym *syn);
    Reference(std::string value);
};