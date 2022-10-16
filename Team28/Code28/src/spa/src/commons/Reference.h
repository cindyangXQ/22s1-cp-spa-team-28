#pragma once

#include "Synonym.h"
#include "Value.h"

enum class ReferenceType { STMT_REF, ENT_REF, WILDCARD, ATTR_REF };

/*
 * Class encapsulating Reference used in Relationship.
 */
class Reference {
public:
    explicit Reference();
    Reference(Synonym syn);
    Reference(Synonym syn, EntityAttribute attr);
    Reference(std::string value);
    bool isWildcard();
    bool isASynonym();
    ReferenceType getRefType();
    Synonym getSynonym();
    EntityName getEntityName();
    std::string getSynonymName();
    EntityAttribute getAttr();
    Value getValue();
    ValueType getValueType();
    std::string getValueString();

private:
    bool isSynonym;
    ReferenceType type;
    Synonym syn;
    EntityAttribute attr;
    Value value;
};
