#pragma once

#include <iostream>
#include <unordered_set>

#include "TableValue.h"

enum class EntityHeader { NAME };

enum class EntityName {
    PROCEDURE,
    STMT,
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    VARIABLE,
    CONSTANT,
};

typedef std::unordered_set<EntityName> ENTITY_TYPE_MAP;
const ENTITY_TYPE_MAP stmtRefSet = {
    EntityName::STMT,  EntityName::READ, EntityName::PRINT, EntityName::CALL,
    EntityName::WHILE, EntityName::IF,   EntityName::ASSIGN};

const ENTITY_TYPE_MAP entRefSet = {EntityName::VARIABLE, EntityName::CONSTANT,
                                   EntityName::PROCEDURE};

/*
 * Class encapsulating 1 of the following design entities: Procedure, Constant,
 * Variable.
 */
class Entity : public TableValue {
public:
    explicit Entity(std::string name) { this->name = name; };

    std::string getName() { return this->name; };

    bool operator==(const Entity &other) const {
        return this->name == other.name;
    };

    /*
     * Checks whether value of EntityHeader is equal.
     */
    bool isValueEqual(EntityHeader header, const Entity &other) {
        switch (header) {
        case EntityHeader::NAME:
            return this->name == other.name;

        default:
            std::cerr << "Unknown EntityHeader." << std::endl;
        }
    };

private:
    std::string name;
};
