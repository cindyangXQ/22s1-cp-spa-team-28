#include "Reference.h"
#include "../QPS/Error/SemanticError.h"
#include "../QPS/Error/SyntaxError.h"
#include "../QPS/Parser/QueryParserRegex.h"

Reference::Reference() {}
Reference::Reference(Synonym syn) {
    this->isSynonym = true;
    this->syn = syn;
    if (entRefSet.count(syn.entity)) {
        this->type = ReferenceType::ENT_REF;
    } else if (stmtRefSet.count(syn.entity)) {
        this->type = ReferenceType::STMT_REF;
    } else {
        throw SemanticError("Invalid reference type");
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
    return this->value.type == ValueType::WILDCARD;
}
