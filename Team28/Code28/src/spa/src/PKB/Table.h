#pragma once

/*
* An interface for the respective tables to implement the shared store() method
*/
template <typename T>
class Table {
	virtual void store(T* item) = 0;
};
