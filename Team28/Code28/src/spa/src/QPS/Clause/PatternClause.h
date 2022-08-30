#include "../Reference/Synonym.h"
#include "../Reference/EntityReference.h"
#include "../Reference/StatementReference.h"
class PatternClause
{
public:
    Synonym syn;
    EntityReference entRef;
    StatementReference stmtRef;
	PatternClause();
    PatternClause(std::string desc);
    PatternClause(Synonym syn, EntityReference entRef, StatementReference stmtRef);
};