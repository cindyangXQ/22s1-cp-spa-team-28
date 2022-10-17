#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "../../../commons/Constant.h"
#include "../../../commons/Entity.h"
#include "../../../commons/Procedure.h"
#include "../../../commons/Variable.h"
#include "../Table.h"

/*
 * Class encapsulating a Table used for storing Procedures/Variables/Constants.
 */
template <typename T> class NamesTable : public Table {
public:
    /*
     * Stores an entity into NamesTable.
     */
    void store(TableValue *entity) {
        T *ent = static_cast<T *>(entity);
        this->names.insert(ent->getName());
        this->nameEntityMap[ent->getName()] = ent;
    };

    /*
     * Returns all names as a string.
     */
    std::vector<std::string> getAllAsString() {
        std::vector<std::string> result(this->names.begin(), this->names.end());
        return result;
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
     * Gets all names from NamesTable.
     */
    std::unordered_set<std::string> getAll() { return this->names; }

protected:
    std::unordered_set<std::string> names;
    std::map<std::string, T *> nameEntityMap;
};

class ConstantsTable : public NamesTable<Constant> {};
class VariablesTable : public NamesTable<Variable> {};
class ProceduresTable : public NamesTable<Procedure> {
public:
    std::vector<int> getAllStmtNum();
};
