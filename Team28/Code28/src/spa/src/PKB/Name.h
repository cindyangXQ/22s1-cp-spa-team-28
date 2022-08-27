#pragma once

#include <string>

/*
* Class encapsulating a Name, which refers to: procedure, variable, constant.
*/
class Name {
public:
	/*
	* Explicit constructor for a Name.
	*/
	explicit Name(int index, std::string nameString);

	std::string getNameString();

	int getIndex();

	/*
	* Override equality operator for Name to check index and nameString.
	*/
	bool operator==(const Name& other) const;

private:
	std::string nameString;
	int index;
};