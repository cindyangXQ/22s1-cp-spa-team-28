#include <iostream>
#include "../Reference/Reference.h"
enum class RelRef {
    Modifies,
    Uses,
    Parent,
    ParentT,
    Follows,
    FollowsT,
    EMPTY
};
class SuchThatClause
{
public:
    RelRef relationshipType;
    Reference ref_1;
    Reference ref_2;

    SuchThatClause();
    SuchThatClause(RelRef relationshipType, Reference ref_1, Reference ref_2);
};