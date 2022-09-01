#include "SelectEvaluator.h"
#include "../QueryFacade.h"

void SelectEvaluator::evaluate(SelectClause selectCl) {
    if (selectCl.syn.entity == DesignEntity::VARIABLE) {
        // QueryFacade::getAllVariable();
    }
    else if (selectCl.syn.entity == DesignEntity::STATEMENT) {
        // QueryFacade::getAllStatement();
    }
}