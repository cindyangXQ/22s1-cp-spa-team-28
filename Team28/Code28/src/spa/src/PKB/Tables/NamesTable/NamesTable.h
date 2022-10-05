#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <unordered_set>

#include "../../../commons/Constant.h"
#include "../../../commons/Entity.h"
#include "../../../commons/Procedure.h"
#include "../../../commons/Variable.h"
#include "../Table.h"

/*
 * Class encapsulating a Table used for storing Procedures/Variables/Constants.
 */
template <typename T> class NamesTable : public Table<Entity> {
public:
    /*
     * Stores an entity into NamesTable.
     */
    void store(Entity *entity) {
        this->names.insert(entity->getName());
        this->nameEntityMap[entity->getName()] = entity;
    };

    /*
     * Retrieves an entity from NamesTable by Name.
     */
    virtual T *retrieve(const std::string &name) {
        auto key = this->nameEntityMap.find(name);

        if (key == this->nameEntityMap.end()) {
            return nullptr;
        }
        return (T *)key->second;
    };

    /*
     * Gets NamedEntityMap for children.
     * TODO: Consider removing.
     */
    std::map<std::string, Entity *> getNameEntityMap() {
        return this->nameEntityMap;
    };

    /*
     * Gets all names from NamesTable.
     */
    std::unordered_set<std::string> getAll() { return this->names; }

protected:
    std::unordered_set<std::string> names;
    std::map<std::string, Entity *> nameEntityMap;
};

typedef NamesTable<Constant> ConstantsTable;
typedef NamesTable<Variable> VariablesTable;
typedef NamesTable<Procedure> ProceduresTable;
