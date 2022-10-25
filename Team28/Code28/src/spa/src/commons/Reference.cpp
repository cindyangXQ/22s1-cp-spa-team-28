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
        entityAttributeMap.at(syn.getEntityName());
    if (validAttr.count(attr)) {
        this->attr = attr;
    } else {
        throw SemanticError("Invalid entity attribute");
    }
}

Reference::Reference(std::string value) {
    this->isSynonym = false;
    if (std::regex_match(value, WILDCARD_REGEX)) {
        this->type = ReferenceType::WILDCARD;
        this->value = Value(ValueType::WILDCARD, "_");
    } else if (std::regex_match(value, INT_REGEX)) {
        this->type = ReferenceType::STMT_REF;
        this->value = Value(ValueType::STMT_NUM, value);
    } else if (std::regex_match(value, NAME_REGEX)) {
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

bool Reference::isSecondaryAttribute() {
    if (SECONDARY_ATTRIBUTE_MAP.count(getEntityName())) {
        return SECONDARY_ATTRIBUTE_MAP.at(getEntityName()) == attr;
    }
    return false;
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

Reference Reference::getReference(std::string input,
                                  std::vector<Synonym> syns) {

    if (input[0] == '\"' && input.back() == '\"') {
        // Remove " at the start and end
        input = input.substr(1, input.size() - 2);
        input = Utils::trimSpaces(input);
        if (!std::regex_match(input, NAME_REGEX)) {
            throw SyntaxError("Invalid reference format");
        }
        return Reference(input);
    }
    if (std::regex_match(input, INT_REGEX) ||
        std::regex_match(input, WILDCARD_REGEX)) {
        return Reference(input);
    } else if (std::regex_match(input, SYN_REGEX)) {
        Synonym synonym = Synonym::getSynonym(input, syns);
        return Reference(synonym);
    } else if (std::regex_match(input, ATTR_REF_REGEX)) {
        std::smatch matches;
        std::regex_match(input, matches, ATTR_REF_REGEX);
        Synonym synonym = Synonym::getSynonym(matches[1], syns);
        if (!ENTITY_ATTR_MAP.count(matches[2])) {
            throw SyntaxError("Invalid attribute name");
        }
        EntityAttribute attr = ENTITY_ATTR_MAP.at(matches[2]);
        return Reference(synonym, attr);
    }
    throw SyntaxError("Invalid reference format");
}
