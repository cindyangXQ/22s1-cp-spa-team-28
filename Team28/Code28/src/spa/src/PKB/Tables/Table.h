#pragma once

/*
 * An interface of tables which acts as our database.
 */
template <typename T> class Table {
    virtual void store(T *item) = 0;
};
