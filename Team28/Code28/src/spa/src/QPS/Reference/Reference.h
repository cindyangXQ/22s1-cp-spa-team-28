#pragma once

/*
 * Enumerates the different type of relationship.
 */
enum class RelRef {
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