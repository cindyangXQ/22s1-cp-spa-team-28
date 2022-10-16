#pragma once

#include "../../commons/Reference.h"
#include "../../commons/Synonym.h"

enum class PatternType { ASSIGN, WHILE, IF };
typedef std::string Expression;
const std::unordered_map<EntityName, PatternType> entityToPatternTypeMap = {
    {EntityName::ASSIGN, PatternType::ASSIGN},
    {EntityName::WHILE, PatternType::WHILE},
    {EntityName::IF, PatternType::IF},
};

/*
 * Class encapsulating the logic of the pattern clause.
 */
class PatternClause {
public:
    Synonym syn;
    PatternType patternType;
    Reference entRef;
    Expression expression;
    bool isExact;

    PatternClause(){};
    PatternClause(Synonym syn, Reference entRef, Expression expression,
                  bool isExact);
};
