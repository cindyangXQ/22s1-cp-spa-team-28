#pragma once

#include "../QPS/Error/SyntaxError.h"
#include "Synonym.h"
#include "Value.h"

enum class ReferenceType { STMT_REF, ENT_REF, WILDCARD, ATTR_REF };
const std::unordered_map<EntityName, Designation> ENTITY_DESIGNATION_MAP = {
    {EntityName::ASSIGN, Designation::ASSIGN},
    {EntityName::IF, Designation::IF_C},
    {EntityName::WHILE, Designation::WHILE_C},
    {EntityName::STMT, Designation::STMT},
    {EntityName::VARIABLE, Designation::VAR},
    {EntityName::CONSTANT, Designation::CONST},
    {EntityName::PROCEDURE, Designation::PROC},
    {EntityName::CALL, Designation::CALL},
    {EntityName::READ, Designation::STMT},
    {EntityName::PRINT, Designation::STMT}};

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
    bool isSecondaryAttribute();
    bool isASynonym();
    ReferenceType getRefType();
    Synonym getSynonym();
    Designation getDesignation();
    EntityName getEntityName();
    std::string getSynonymName();
    EntityAttribute getAttr();
    Value getValue();
    ValueType getValueType();
    std::string getValueString();

    static Reference getReference(std::string input, std::vector<Synonym> syns);

private:
    bool isSynonym;
    ReferenceType type;
    Synonym syn;
    EntityAttribute attr;
    Value value;
};