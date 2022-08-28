#pragma once

template <typename Name>
class Entity {
public:
	explicit Entity(Name name) {
		this->name = name;
	};

	Name getName() {
		return this->name;
	};

	bool operator==(const Entity& other) const {
		return this->name == other.name;
	};

private:
	Name name;
};