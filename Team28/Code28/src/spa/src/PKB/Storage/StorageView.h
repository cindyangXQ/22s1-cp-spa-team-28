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

#include "../Tables/NamesTable/NamesTable.h"
#include "../Tables/StatementsTable/StatementsTable.h"
#include "../Tables/Table.h"

/*
 * Encapsulates a StorageView class which is a read-only Storage that allows
 * other tables in PKB to retrieve values from entities, statements and proc tables.
 */
class StorageView {
public:
    /*
     * Explicit constructor for Storage.
     */
    StorageView() {};

    /*
     * Retrieve a table by the templated class given
     */
    template <typename Subclass> Subclass *getTable() {
        Table *table = this->tables.at(typeid(Subclass));
        return dynamic_cast<Subclass *>(table);
    };


    /*
     * Sets a pointer to a table by the templated class given
     */
    template <typename Subclass> void *setTable(Table *table) {
        this->tables[typeid(Subclass)] = table;
    };

private:
    std::map<std::type_index, Table *> tables;
};
