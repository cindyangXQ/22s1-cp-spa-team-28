#pragma once

#include "Value.h"
#include "Synonym.h"

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
    Synonym syn;
    Value value;
    explicit Reference();
    Reference(Synonym syn);
    Reference(std::string value);
    bool isWildcard();
};