#pragma once

#include <vector>

#include "../Storage/Storage.h"

/*
 * A Facade class for SP->PKB interactions.
 */
class PopulateFacade {
public:
    /*
     * Explicit constructor for a PopulateFacade.
     */
    explicit PopulateFacade(Storage *storage);

    /*
     * Generic store method for SP->PKB interactions.
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
