#pragma once

#include "Synonym.h"
#include "Value.h"

enum class ReferenceType { STMT_REF, ENT_REF, WILDCARD, ATTR_REF };

/*
 * Class encapsulating Reference used in Relationship.
 */
class Reference {
public:
    ReferenceType type;
    bool isSynonym;
    Synonym syn;
    EntityAttribute attr;
    Value value;
    explicit Reference();
    Reference(Synonym syn);
    Reference(Synonym syn, EntityAttribute attr);
    Reference(std::string value);
    bool isWildcard();
    EntityName getEntityName();
    bool isInferredAttribute();
};
