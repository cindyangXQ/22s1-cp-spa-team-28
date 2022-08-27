#include "Synonym.h"
#include "../Utils.h"

Synonym::Synonym() {}
Synonym::Synonym(DesignEntity entity, string name) {
	this->entity = entity;
    this->name = name;
}
Synonym::Synonym(string s) {
    vector<string> tokens = Utils::splitString(s, ' ');
    if (tokens[0].compare("Procedure") == 0) {
        this->entity = PROCEDURE;
    }
    else if (tokens[0].compare("Statement") == 0) {
        this->entity = STATEMENT;
    }
    else if (tokens[0].compare("Read") == 0) {
        this->entity = READ;
    }
    else if (tokens[0].compare("Print") == 0) {
        this->entity = PRINT;
    }
    else if (tokens[0].compare("Assign") == 0) {
        this->entity = ASSIGN;
    }
    else if (tokens[0].compare("Call") == 0) {
        this->entity = CALL;
    }
    else if (tokens[0].compare("While") == 0) {
        this->entity = WHILE;
    }
    else if (tokens[0].compare("If") == 0) {
        this->entity = IF;
    }
    else if (tokens[0].compare("Variable") == 0) {
        this->entity = VARIABLE;
    }
    else if (tokens[0].compare("Constant") == 0) {
        this->entity = CONSTANT;
    }
    this->name = tokens[1];
}
