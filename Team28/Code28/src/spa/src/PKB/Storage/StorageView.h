#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "../../commons/Constant.h"
#include "../../commons/Entity.h"
#include "../../commons/Procedure.h"
#include "../../commons/TableValue.h"
#include "../../commons/Variable.h"
#include "../Tables/Table.h"

/*
 * Encapsulates a read-only Storage that allows other tables in PKB to retrieve
 * values from Entities, Statements and Procedure tables.
 */
class StorageView {
public:
    /*
     * Constructor for Storage.
     */
    StorageView(){};

    /*
     * Retrieves a table by the templated class given.
     */
    template <typename Subclass> Subclass *getTable() {
        Table *table = this->tables.at(typeid(Subclass));
        return dynamic_cast<Subclass *>(table);
    };

    /*
     * Sets a pointer to a table by the templated class given.
     */
    template <typename Subclass> void setTable(Table *table) {
        this->tables[typeid(Subclass)] = table;
    };

private:
    std::map<std::type_index, Table *> tables;
};
