#include "SelectClause.h"

SelectClause::SelectClause() {}
SelectClause::SelectClause(std::string desc) {

}
SelectClause::SelectClause(Synonym syn) {
	this->syn = syn;
}