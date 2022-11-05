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
#include "../Attributable.h"
#include "../Table.h"

/*
 * Class encapsulating a Table used for storing Procedures/Variables/Constants.
 */
template <typename T> class NamesTable : public Table, public Attributable {
public:
    /*
     * Stores an entity into NamesTable.
     */
    void store(TableValue *entity) {
        T *ent = static_cast<T *>(entity);
        this->names.insert(ent->getName());
        this->nameEntityMap[ent->getName()] = ent;
        this->tableSize++;
    };

    int getTableSize() { return this->tableSize; }

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

    virtual std::vector<Value> getMatchingValue(std::string value,
                                                EntityName entity,
                                                StorageView *storage) {
        UNUSED(entity);
        UNUSED(storage);
        std::vector<Value> result = {};
        if (this->names.count(value) == 1) {
            result.push_back(Value(ValueType::VAR_NAME, value));
        }
        return result;
    }

    virtual std::map<Value, std::vector<Value>>
    getAllValues(EntityName entity, StorageView *storage) {
        UNUSED(entity);
        UNUSED(storage);
        std::map<Value, std::vector<Value>> result = {};
        for (std::string name : this->names) {
            Value v = Value(ValueType::VAR_NAME, name);
            result[v] = {v};
        }
        return result;
    };

protected:
    std::unordered_set<std::string> names;
    std::map<std::string, T *> nameEntityMap;
    int tableSize = 0;
};

class ConstantsTable : public NamesTable<Constant> {
public:
    std::vector<Value> getMatchingValue(std::string value, EntityName entity,
                                        StorageView *storage);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity,
                                                     StorageView *storage);
};
class VariablesTable : public NamesTable<Variable> {};
class ProceduresTable : public NamesTable<Procedure> {
public:
    std::vector<int> getAllStmtNum();
};
