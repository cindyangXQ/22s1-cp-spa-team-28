#include <iostream>
#include "../Reference/Reference.h"
enum class RelRef {
    Modifies,
    Uses,
    Parent,
    ParentT,
    Follows,
    FollowsT,
    Empty
};
class SuchThatClause
{
public:
    RelRef relationshipType;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelRef relationshipType, Reference refLeft, Reference refRight;
};