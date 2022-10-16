#include "Reference.h"
#include "../QPS/Error/SemanticError.h"
#include "../QPS/Error/SyntaxError.h"
#include "../QPS/Parser/QueryParserRegex.h"

Reference::Reference() {}
Reference::Reference(Synonym syn) {
    this->isSynonym = true;
    this->syn = syn;
    if (entRefSet.count(syn.getEntityName())) {
        this->type = ReferenceType::ENT_REF;
    } else if (stmtRefSet.count(syn.getEntityName())) {
        this->type = ReferenceType::STMT_REF;
    } else {
        throw SemanticError("Invalid reference type");
    }
}

Reference::Reference(Synonym syn, EntityAttribute attr) {
    this->isSynonym = false;
    this->type = ReferenceType::ATTR_REF;
    this->syn = syn;
    std::unordered_set<EntityAttribute> validAttr =
        entityAttributeMap.find(syn.getEntityName())->second;
    if (validAttr.count(attr)) {
        this->attr = attr;
    } else {
        throw SemanticError("Invalid entity attribute");
    }
}

Reference::Reference(std::string value) {
    this->isSynonym = false;
    if (std::regex_match(value, wildcardRegex)) {
        this->type = ReferenceType::WILDCARD;
        this->value = Value(ValueType::WILDCARD, "_");
    } else if (std::regex_match(value, intRegex)) {
        this->type = ReferenceType::STMT_REF;
        this->value = Value(ValueType::STMT_NUM, value);
    } else if (std::regex_match(value, nameRegex)) {
        this->type = ReferenceType::ENT_REF;
        this->value = Value(ValueType::VAR_NAME, value);
    } else {
        throw SyntaxError("Invalid reference format");
    }
}

bool Reference::isWildcard() {
    if (this->isSynonym) {
        return false;
    }
    return this->value.getValueType() == ValueType::WILDCARD;
}

bool Reference::isASynonym() { return this->isSynonym; }

ReferenceType Reference::getRefType() { return this->type; }

Synonym Reference::getSynonym() { return this->syn; }

EntityName Reference::getEntityName() { return this->syn.getEntityName(); }

std::string Reference::getSynonymName() { return this->syn.getName(); }

EntityAttribute Reference::getAttr() { return this->attr; }

Value Reference::getValue() { return this->value; }

ValueType Reference::getValueType() { return this->value.getValueType(); }

std::string Reference::getValueString() { return this->value.getValue(); }
