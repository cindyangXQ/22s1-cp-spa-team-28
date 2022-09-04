#pragma once

/*
 * Enumerates the different type of relationship.
 */
enum class RelationshipReference {
    Modifies,
    Uses,
    Parent,
    ParentT,
    Follows,
    FollowsT,
    Empty
};

class Reference {
	public:
};