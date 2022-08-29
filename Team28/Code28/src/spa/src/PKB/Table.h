#pragma once

#include "PredicateMap.h"

/*
* An interface of tables which acts as our database
*/
template <typename T>
class Table {
	virtual void store(T* item) = 0;

	virtual int getTableSize() const;

	/**
	 * @brief Filters the table based on the Predicate
	 * 
	 * @param predicate Predicate used to filter content in the Table
	 * @return Table* Pointer to another table where the contents are filtered
	 */
	virtual Table *filter(PredicateMap<T,U> predicate);
};
