#pragma once

#include <vector>

#include "../Storage/Storage.h"

/*
 * Encapsulates a Facade class for SP->PKB interactions.
 */
class PopulateFacade {
public:
    /*
     * Explicit constructor for PopulateFacade.
     */
    explicit PopulateFacade(Storage *storage);

    /*
     * Stores all items of a given type T into the corresponding table specified
     * by popType.
     */
    template <typename T>
    void store(std::vector<T *> *items, Designation popType) {
        Table *table = this->storage->getDesignationTable(popType);
        for (T *item : *items) {
            table->store(item);
        }
    };

private:
    Storage *storage;
};
